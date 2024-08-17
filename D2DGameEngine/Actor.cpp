#include "Actor.h"
#include "SceneComponent.h"

#include "PrimitiveComponent.h"
#include "Controller.h"

#include "DamageEvent.h"

#include "World.h"

#include "Ray.h"

Actor::Actor(class World* _world) : world(_world)
{
}

Actor::~Actor()
{
	for (auto [_, component] : components) {
		PrimitiveComponent* primComp = dynamic_cast<PrimitiveComponent*>(component);
		if (primComp) GetWorld()->UnregisterComponentCollision(primComp);
		delete component;
		component = nullptr;
	}
}


float Actor::TakeDamage(float damageAmount, DamageEvent const& damageEvent, Controller* eventInstigator, Actor* damageCauser)
{
	float actualDamage = InternalTakeDamage(damageAmount, damageEvent, eventInstigator, damageCauser);

	if (damageEvent.damageEventType == DamageEventType::PointDamage)
	{
		const PointDamageEvent& pointDamageEvent = static_cast<const PointDamageEvent&>(damageEvent);

		if (abs(actualDamage) > EPSILON) 
		{
			PrimitiveComponent* const primComp = pointDamageEvent.hitInfo.hitComponent;

			if (primComp)
			{
				primComp->ReceiveComponentDamage(damageAmount, damageEvent, eventInstigator, damageCauser);
			}
		}
	}
	else if (damageEvent.damageEventType == DamageEventType::RadialDamage)
	{
		const RadialDamageEvent& radialDamageEvent = static_cast<const RadialDamageEvent&>(damageEvent);
		// TODO: Internal Radial Take Damage.
		if (abs(actualDamage) > EPSILON) 
		{
			for (int hitIndex = 0; hitIndex < radialDamageEvent.componentHits.size(); ++hitIndex)
			{
				const HitResult& compHitResult = radialDamageEvent.componentHits[hitIndex];
				PrimitiveComponent* const primComp = compHitResult.hitComponent;
				if (primComp && primComp->GetOwner() == this)
				{
					primComp->ReceiveComponentDamage(damageAmount, damageEvent, eventInstigator, damageCauser);
				}
			}
		}
	}

	if (abs(actualDamage) > EPSILON) {
		OnTakeDamage(actualDamage, damageEvent, eventInstigator, damageCauser);

		/*if (eventInstigator != nullptr)
		{
			eventInstigator->InstigatedAnyDamage(actualDamage, damageEvent.damageEventType, this, damageCauser);
		}*/
	}

	return actualDamage;
}

void Actor::BeginPlay()
{
	for (auto [_, component] : components) {
		if(component->GetStatus() == OS_AWAKE)
			component->BeginPlay();
	}
	status = OS_ACTIVE;
}

bool Actor::Destroy()
{
	if (status == OS_DESTROY)
		return false;
	status = OS_DESTROY;
	return true;
}

void Actor::FixedUpdate(float _fixedRate)
{
	for (auto [_, component] : components) {
		if(component->CheckTickProperty(TICK_PHYSICS) && component->GetStatus() == OS_ACTIVE)
			component->FixedUpdate(_fixedRate);
	}
}

void Actor::PreUpdate(float _dt)
{
	for (auto [_, component] : components) {
		if (component->CheckTickProperty(TICK_PRE_UPDATE) && component->GetStatus() == OS_ACTIVE)
			component->PreUpdate(_dt);
	}
}

void Actor::Update(float _dt)
{
	for (auto [_, component] : components) {
		if (component->CheckTickProperty(TICK_UPDATE) && component->GetStatus() == OS_ACTIVE)
			component->Update(_dt);
	}
}

void Actor::PostUpdate(float _dt)
{
	for (auto [_, component] : components) {
		if (component->CheckTickProperty(TICK_POST_UPDATE) && component->GetStatus() == OS_ACTIVE)
			component->PostUpdate(_dt);
	}
}

void Actor::PrepareRender()
{
	// Render dirty set
	std::vector<class PrimitiveComponent*> renderDirtyComponentSet;

	// Render dirty flag set
	for (auto it = renderSequence.begin(); it != renderSequence.end();)
	{
		auto [prevY, primComp] = *it;

		if (EpsilonEquals(prevY, primComp->GetComponentLocation().y)) {
			++it;
			continue;
		};

		primComp->bRenderDirty = true;
		renderDirtyComponentSet.push_back(primComp);
		it = renderSequence.erase(it);
	}

	for (PrimitiveComponent* primComp : renderDirtyComponentSet) 
	{
		renderSequence.insert({ primComp->GetComponentLocation().y, primComp });
	}
}

void Actor::Render(D2DRenderer* _renderer)
{
	for (auto [_, primComp] : renderSequence)
	{
		if (primComp->isVisible &&
			primComp->CheckTickProperty(TICK_RENDER) &&
			primComp->GetStatus() == OS_ACTIVE)
			primComp->Render(_renderer);
	}
}

void Actor::SetLocation(const float x, const float y)
{
	if (rootComponent != nullptr)
		rootComponent->SetTranslation(x, y);
}

Math::Vector2 Actor::GetLocation() const
{
	if (rootComponent != nullptr)
	{
		return rootComponent->GetComponentLocation();
	}
	return { 0, 0 };
}

void Actor::SetRotation(const float _degree)
{
	if (rootComponent != nullptr) 
		rootComponent->SetRotation(_degree);
}

void Actor::SetScale(const float x, const float y)
{
	if (rootComponent != nullptr) 
		rootComponent->SetScale(x, y);
}

Math::Matrix Actor::GetTrasnform() const
{
	if (rootComponent != nullptr)
		return rootComponent->GetWorldTransform();
	else
		return Math::Matrix::Identity;
}

void Actor::RotateToward(const Math::Vector2& _direction)
{
	Math::Vector2 w = _direction;
	Math::Vector2 v = Right();

	float x = w.y * v.x - w.x * v.y;
	float y = w.x * v.x + w.y * v.y;
	float rad = atan2(x, y);

	SetRotation(rad);
}

void Actor::LookAt(const Math::Vector2& _point)
{
	Math::Vector2 loc = GetLocation();
	Math::Vector2 direction = _point - loc;
	direction.Normalize();
	RotateToward(direction);
}
