#include "TighteningCircleEffect.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/AnimationBitmapComponent.h"
#include "../D2DGameEngine/CircleComponent.h"

TighteningCircleEffect::TighteningCircleEffect(World* _world) : SkillActor(_world)
{
	SetTickProperties(TICK_PHYSICS | TICK_UPDATE | TICK_RENDER);

	rootComponent = abm = CreateComponent<AnimationBitmapComponent>();
	abm->SetSprite(L"TestResource/Player/Skill/Skill_TighteningCircle.png");
	abm->SliceSpriteSheet(650, 500, 0, 0, 0, 0);
	abm->SetFrameDurations({ 2.f/36 });

	circleComponent = CreateComponent<CircleComponent>();
	circleComponent->collisionProperty = CollisionProperty(CollisionPropertyPreset::PlayerPattern);	// 오브젝트의 충돌 채널은 WorldStatic, 모든 충돌 채널에 대한 반응은 `Block`.	// 오브젝트의 충돌 채널은 WorldStatic, 모든 충돌 채널에 대한 반응은 `Block`.
	circleComponent->bSimulatePhysics = false;				// 움직임에 물리를 적용합니다.
	circleComponent->bApplyImpulseOnDamage = true;	// 데미지를 받을 때 충격을 가합니다.
	circleComponent->bGenerateOverlapEvent = true;	// Overlap 이벤트를 발생시킵니다.
	abm->AddChild(circleComponent);

	DamageType radialDamageType{
		.damageImpulse = 10000.f,
	};
	tighteningDamageEvent.SetDamageType(radialDamageType);
	tighteningDamageEvent.origin = GetLocation();
	tighteningDamageEvent.damageEventType = DamageEventType::RadialDamage;
	tighteningDamageEvent.radialDamageInfo.maxDamage = damage;
	tighteningDamageEvent.radialDamageInfo.minDamage = damage;
	tighteningDamageEvent.radialDamageInfo.innerRadius = radius;
	tighteningDamageEvent.radialDamageInfo.outerRadius = radius;
	tighteningDamageEvent.componentHits.resize(1);

}

void TighteningCircleEffect::BeginPlay()
{
	__super::BeginPlay();
	Inactivate();
}

void TighteningCircleEffect::Initialize()
{
	scaleTween = new DotTween<float>(&scaleVarias, EasingEffect::Linear, StepAnimation::StepOnceForward);
	scaleTween->SetDuration(2.f);
	scaleTween->SetStartPoint(1.f);
	scaleTween->SetEndPoint(0.f);
	circleComponent->InitCircleRadius(radius);
	abm->Trigger(true);
	abm->InitFrame();

}

void TighteningCircleEffect::Update(float _dt)
{
	__super::Update(_dt);

	scaleTween->Update(_dt);
	damageTimer += _dt;
	elapsedTime += _dt;

	circleComponent->bShouldOverlapTest = true;

	circleComponent->SetCircleRadius(radius * scaleVarias);
	tighteningDamageEvent.radialDamageInfo.innerRadius = radius * scaleVarias - 10;
	tighteningDamageEvent.radialDamageInfo.outerRadius = radius * scaleVarias;

	if (elapsedTime > skillDuration)
	{
		Inactivate();
		elapsedTime = 0.f;
		delete scaleTween;
	}
}

void TighteningCircleEffect::OnOverlap(Actor* other, const OverlapInfo& overlap)
{
	__super::OnOverlap(other, overlap);

	if (damageTimer > tickInterval)
	{
		bool hitRadius = intersectionUtil::BoundaryCircleBoxIntersect(
			circleComponent->CalculateLocalBounds().GetCircle(),
			overlap.overlapInfo.hitComponent->CalculateBounds(overlap.overlapInfo.hitComponent->GetWorldTransform()).GetBox()
		);
		if (!hitRadius)
			return;

		damageTimer = 0.f;
		other->TakeDamage(
			damage,
			tighteningDamageEvent,
			nullptr,
			this
		);
	}
}
