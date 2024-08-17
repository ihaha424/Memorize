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

BossGrowCircle::BossGrowCircle(World* _world)
	:BossSkillActor(_world)
{
	ReflectionIn();

	circleComponent = CreateComponent<CircleComponent>();

	circleComponent->collisionProperty = CollisionProperty(CollisionPropertyPreset::Enemy);	// 오브젝트의 충돌 채널은 WorldStatic, 모든 충돌 채널에 대한 반응은 `Block`.
	circleComponent->bSimulatePhysics = false;				// 움직임에 물리를 적용합니다.
	circleComponent->bApplyImpulseOnDamage = true;	// 데미지를 받을 때 충격을 가합니다.
	circleComponent->bGenerateOverlapEvent = true;	// Overlap 이벤트를 발생시킵니다.
	circleComponent;	// 게임 오브젝트의 루트 컴포넌트가 충돌체 입니다.
	bm->AddChild(circleComponent);

	DamageType radiaDamageType{
		.damageImpulse = 10000.f,
	};
	BossGrowCircleDamageEvent.SetDamageType(radiaDamageType);
	BossGrowCircleDamageEvent.origin = GetLocation();
	BossGrowCircleDamageEvent.damageEventType = DamageEventType::RadialDamage;
	BossGrowCircleDamageEvent.radialDamageInfo.maxDamage = 0.f;
	BossGrowCircleDamageEvent.radialDamageInfo.minDamage = damage;
	BossGrowCircleDamageEvent.radialDamageInfo.innerRadius = 749.f;
	BossGrowCircleDamageEvent.radialDamageInfo.outerRadius = 750.f;
	BossGrowCircleDamageEvent.componentHits.resize(1);

	scaleTween = new DotTween<float>(&scaleVarias, EasingEffect::Linear, StepAnimation::StepOnceForward);
	scaleTween->SetDuration(10.f);
	skillDuration = 10.f;
	scaleTween->SetStartPoint(1.f);
	scaleTween->SetEndPoint(0.f);
}

void BossGrowCircle::BeginPlay()
{
	__super::BeginPlay();
	bm->SetSprite(L"TestResource/Boss/MagicCircle/Pattern06_MagicCircle.png");
	circleComponent->InitCircleRadius(bm->GetSpriteHeight() / 2);	// 반지름이 62이고 높이가 110 인 캡슐 충돌체를 초기화 합니다.
	//circleComponent->SetStatus(EObjectStatus::OS_INACTIVE);

	player = GetWorld()->FindActorByType<Player>();
}

void BossGrowCircle::FixedUpdate(float _fixedRate)
{
	__super::FixedUpdate(_fixedRate);
	circleComponent->bShouldOverlapTest = true;
}

void BossGrowCircle::Update(float _dt)
{
	__super::Update(_dt);

	if (startSkill < 1.f)
	{
		startSkill += _dt;
		return;
	}
	skillDuration -= _dt;

	scaleTween->Update(_dt);
	bm->SetScale(scaleVarias, scaleVarias);
	if (skillDuration < 0.f)
	{
		SetStatus(EObjectStatus::OS_DESTROY);
	}
}

void BossGrowCircle::OnBeginOverlap(Actor* other, const OverlapInfo& overlap)
{
	BossGrowCircleDamageEvent.radialDamageInfo.innerRadius = 750.f * scaleVarias;
	BossGrowCircleDamageEvent.radialDamageInfo.outerRadius = 750.f * scaleVarias;
	other->TakeDamage(damage, BossGrowCircleDamageEvent, nullptr, this);
}

void BossGrowCircle::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PatternData/Pattern02_Actor.txt");
	reflectionResource->ParsingFile(0, skillDuration, damage);
}

void BossGrowCircle::ReflectionOut()
{
}
