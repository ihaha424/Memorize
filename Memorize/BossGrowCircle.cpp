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

BossGrowCircle::BossGrowCircle(World* _world)
	:BossSkillActor(_world)
{
	ReflectionIn();

	abm = CreateComponent<AnimationBitmapComponent>();
	rootComponent->AddChild(abm);
	bm->isVisible = false;

	circleComponent = CreateComponent<CircleComponent>();

	circleComponent->collisionProperty = CollisionProperty(CollisionPropertyPreset::EnemyPattern);	// ������Ʈ�� �浹 ä���� WorldStatic, ��� �浹 ä�ο� ���� ������ `Block`.
	circleComponent->bSimulatePhysics = false;				// �����ӿ� ������ �����մϴ�.
	circleComponent->bApplyImpulseOnDamage = true;	// �������� ���� �� ����� ���մϴ�.
	circleComponent->bGenerateOverlapEvent = true;	// Overlap �̺�Ʈ�� �߻���ŵ�ϴ�.
	circleComponent;	// ���� ������Ʈ�� ��Ʈ ������Ʈ�� �浹ü �Դϴ�.
	bm->AddChild(circleComponent);

	DamageType radiaDamageType{
		.damageImpulse = 10000.f,
	};
	BossGrowCircleDamageEvent.SetDamageType(radiaDamageType);
	BossGrowCircleDamageEvent.origin = GetLocation();
	BossGrowCircleDamageEvent.damageEventType = DamageEventType::RadialDamage;
	BossGrowCircleDamageEvent.radialDamageInfo.maxDamage = damage;
	BossGrowCircleDamageEvent.radialDamageInfo.minDamage = damage;
	BossGrowCircleDamageEvent.radialDamageInfo.innerRadius = radius;
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

	//circleComponent->SetStatus(EObjectStatus::OS_INACTIVE);

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

	circleComponent->SetCircleRadius(radius * scaleVarias);
	circleComponent->bShouldOverlapTest = true;
	for (auto& [actor, f] : tickDamageTimerMap)
	{
		// ƽ ������ ������Ʈ
		f.Update(_dt);
	}
	if (skillDuration < 0.f)
	{
		SetStatus(EObjectStatus::OS_DESTROY);
	}
}

void BossGrowCircle::OnBeginOverlap(Actor* other, const OverlapInfo& overlap)
{
	if (other)
	{
		bool hitRadius = intersectionUtil::BoundaryCircleBoxIntersect(
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
		tickDamageTimerMap.insert({ other, TakeDamageTimer(f, tickInterval, true) });
	}
}

void BossGrowCircle::OnEndOverlap(Actor* other, const OverlapInfo& overlap)
{
	if (other)
	{
		// ƽ ������ ����
		auto it = tickDamageTimerMap.find(other);
		if (it != tickDamageTimerMap.end())
		{
			it->second.SetFinish(true);
			tickDamageTimerMap.erase(it);
		}
	}
}

void BossGrowCircle::SetGrowLess(bool _Grow)
{
	if (_Grow)
	{
		scaleTween->SetStartPoint(1.f);
		scaleTween->SetEndPoint(0.f);
		circleComponent->InitCircleRadius(radius);
	}
	else
	{
		scaleTween->SetStartPoint(0.f);
		scaleTween->SetEndPoint(1.f);
		circleComponent->InitCircleRadius(0.f);
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
