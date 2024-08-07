#include "Actor.h"
#include "SceneComponent.h"

Actor::Actor(class World* _world) : world(_world)
{
}

Actor::~Actor()
{
	for (auto [_, component] : components) {
		delete component;
		component = nullptr;
	}
}


void Actor::BeginPlay()
{
	for (auto [_, component] : components) {
		component->BeginPlay();
	}
	status = AS_ACTIVE;
}

bool Actor::Destroy()
{
	if (status == AS_DESTROY)
		return false;
	status = AS_DESTROY;
	return true;
}

void Actor::FixedUpdate(float _fixedRate)
{
	for (auto [_, component] : components) {
		if(component->CheckTickProperty(TICK_PHYSICS))
			component->FixedUpdate(_fixedRate);
	}
}

void Actor::PreUpdate(float _dt)
{
	for (auto [_, component] : components) {
		if (component->CheckTickProperty(TICK_PRE_UPDATE))
			component->PreUpdate(_dt);
	}
}

void Actor::Update(float _dt)
{
	for (auto [_, component] : components) {
		if (component->CheckTickProperty(TICK_UPDATE))
			component->Update(_dt);
	}
}

void Actor::PostUpdate(float _dt)
{
	for (auto [_, component] : components) {
		if (component->CheckTickProperty(TICK_POST_UPDATE))
			component->PostUpdate(_dt);
	}
}

void Actor::Render(D2DRenderer* _renderer)
{
	for (auto [_, component] : components) {
		if (component->CheckTickProperty(TICK_RENDER))
			component->Render(_renderer);
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
		D2D_Point2F pos = rootComponent->GetComponentLocation();
		return { pos.x, pos.y };
	}
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

D2D_TMat3x2F Actor::GetTrasnform() const
{
	if (rootComponent != nullptr)
		return rootComponent->GetWorldTransform();
}
