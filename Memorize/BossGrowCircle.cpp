#include "BossGrowCircle.h"
#include "../D2DGameEngine/ReflectionResource.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/AnimationBitmapComponent.h"
#include "MovementComponent.h"
#include "../D2DGameEngine/CircleComponent.h"
#include "../D2DGameEngine/EventBus.h"
#include "TestLevel1_RenderLayer.h"
#include "D2DGameEngine/World.h"
#include "Player.h"
#include "D2DGameEngine/IntersectionUtil.h"
#include "D2DGameEngine/D2DRenderer.h"
#include "D2DGameEngine/BoxComponent.h"

BossGrowCircle::BossGrowCircle(World* _world)
	:BossSkillActor(_world)
{
	ReflectionIn();

	abm = CreateComponent<AnimationBitmapComponent>();
	rootComponent->AddChild(abm);
	bm->isVisible = false;

	//circleComponent = CreateComponent<CircleComponent>();

	//circleComponent->collisionProperty = CollisionProperty(CollisionPropertyPreset::EnemyPattern);	// ������Ʈ�� �浹 ä���� WorldStatic, ��� �浹 ä�ο� ���� ������ `Block`.
	//circleComponent->bSimulatePhysics = false;				// �����ӿ� ������ �����մϴ�.
	//circleComponent->bApplyImpulseOnDamage = true;	// �������� ���� �� ����� ���մϴ�.
	//circleComponent->bGenerateOverlapEvent = true;	// Overlap �̺�Ʈ�� �߻���ŵ�ϴ�.
	//bm->AddChild(circleComponent);



	DamageType radiaDamageType{
		.damageImpulse = 10000.f,
	};
	BossGrowCircleDamageEvent.SetDamageType(radiaDamageType);
	BossGrowCircleDamageEvent.origin = GetLocation();
	BossGrowCircleDamageEvent.damageEventType = DamageEventType::RadialDamage;
	BossGrowCircleDamageEvent.radialDamageInfo.maxDamage = damage;
	BossGrowCircleDamageEvent.radialDamageInfo.minDamage = damage;
	BossGrowCircleDamageEvent.radialDamageInfo.innerRadius = radius - 1;
	BossGrowCircleDamageEvent.radialDamageInfo.outerRadius = radius;
	BossGrowCircleDamageEvent.componentHits.resize(1);

	scaleTween = new DotTween<float>(&scaleVarias, EasingEffect::Linear, StepAnimation::StepOnceForward);
	scaleTween->SetDuration(2.f);
}

void BossGrowCircle::BeginPlay()
{
	__super::BeginPlay();

	{
		abm->SetSprite(L"TestResource/Boss/MagicCircle/BossGrowCircle.png");
		abm->SliceSpriteSheet(1300, 1000, 0, 0, 0, 0);
		abm->SetFrameDurations({ 0.084f });
		abm->SetReverse(isReverse);
		abm->Trigger(false);
		abm->Trigger(true);
	}

	//circleComponent->SetStatus(EObjectStatus::OS_ACTIVE);

	player = GetWorld()->FindActorByType<Player>();
}

void BossGrowCircle::FixedUpdate(float _fixedRate)
{
	__super::FixedUpdate(_fixedRate);
}

void BossGrowCircle::Update(float _dt)
{
	__super::Update(_dt);

	if (startSkill > 0.f)
	{
		startSkill -= _dt;
		return;
	}
	skillDuration -= _dt;
	scaleTween->Update(_dt);
	//bm->SetScale(scaleVarias, scaleVarias);

	ellipse.center = GetLocation();
	ellipse.minor = minor * scaleVarias;
	ellipse.major = major * scaleVarias;
	/*circleComponent->SetCircleRadius(radius * scaleVarias);
	circleComponent->bShouldOverlapTest = true;*/

	bool res = ellipse.CheckIntersectWithBox(player->collisionBox->bounds.GetBox());
	if (res)
	{
		auto it = tickDamageTimerMap.find(player);
		if (it == tickDamageTimerMap.end())
		{
			auto f = [this]() {
				BossGrowCircleDamageEvent.radialDamageInfo.innerRadius = radius * scaleVarias;
				BossGrowCircleDamageEvent.radialDamageInfo.outerRadius = radius * scaleVarias;
				player->TakeDamage(
					damage,
					BossGrowCircleDamageEvent,
					nullptr,
					this
				);
				};
			f();
			tickDamageTimerMap.insert({ player, TakeDamageTimer(f, tickInterval, true) });
		}
	}
	else
	{
		auto it = tickDamageTimerMap.find(player);
		if (it != tickDamageTimerMap.end())
		{
			it->second.SetFinish(true);
			tickDamageTimerMap.erase(it);
		}
	}

	for (auto& [actor, f] : tickDamageTimerMap)
	{
		// ƽ ������ ������Ʈ
		f.Update(_dt);
	}
	if (skillDuration < 0.f)
	{
		Destroy();
	}
}

void BossGrowCircle::Render(D2DRenderer* _renderer)
{
	Super::Render(_renderer);

#ifndef NDEBUG
	_renderer->DrawEllipse(ellipse.center, ellipse.major, ellipse.minor, D2D_Color::Red);
#endif // !NDEBUG
}

bool BossGrowCircle::Destroy()
{
	return __super::Destroy();
}

void BossGrowCircle::OnBeginOverlap(Actor* other, const OverlapInfo& overlap)
{
	if (other)
	{
		/*bool hitRadius = intersectionUtil::BoundaryCircleBoxIntersect(
			circleComponent->CalculateLocalBounds().GetCircle(),
			overlap.overlapInfo.hitComponent->CalculateBounds(overlap.overlapInfo.hitComponent->GetWorldTransform()).GetBox()
		);
		if (!hitRadius)
			return;
		auto f = [this, other]() {
			BossGrowCircleDamageEvent.radialDamageInfo.innerRadius = radius * scaleVarias;
			BossGrowCircleDamageEvent.radialDamageInfo.outerRadius = radius * scaleVarias;
			other->TakeDamage(
				damage,
				BossGrowCircleDamageEvent,
				nullptr,
				this
			);
		};
		f();
		tickDamageTimerMap.insert({ other, TakeDamageTimer(f, tickInterval, true) });*/
	}
}

void BossGrowCircle::OnEndOverlap(Actor* other, const OverlapInfo& overlap)
{
	if (other)
	{
		// ƽ ������ ����
		/*auto it = tickDamageTimerMap.find(other);
		if (it != tickDamageTimerMap.end())
		{
			it->second.SetFinish(true);
			tickDamageTimerMap.erase(it);
		}*/
	}
}

void BossGrowCircle::SetGrowLess(bool _Grow)
{
	if (_Grow)
	{
		scaleTween->SetStartPoint(1.f);
		scaleTween->SetEndPoint(0.f);
		//circleComponent->InitCircleRadius(radius);
	}
	else
	{
		scaleTween->SetStartPoint(0.f);
		scaleTween->SetEndPoint(1.f);
		//circleComponent->InitCircleRadius(0.f);
		isReverse = true;
	}
}


void BossGrowCircle::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PatternData/Pattern02_Actor.txt");
	reflectionResource->ParsingFile(0, skillDuration, damage);
}

void BossGrowCircle::ReflectionOut()
{
}
