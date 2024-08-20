#include "BossChaseCircle.h"
#include "../D2DGameEngine/ReflectionResource.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "MovementComponent.h"
#include "../D2DGameEngine/ClickComponent.h"
#include "../D2DGameEngine/CircleComponent.h"
#include "../D2DGameEngine/EventBus.h"

#include "D2DGameEngine/World.h"
#include "Player.h"

BossChaseCircle::BossChaseCircle(World* _world)
	:BossSkillActor(_world)
{
	ReflectionIn();
	disfellCommandCount = 2;
	CreateDisfellCommand();
	CreateComponent<ClickComponent>();

	circleComponent = CreateComponent<CircleComponent>();

	circleComponent->collisionProperty = CollisionProperty(CollisionPropertyPreset::Enemy);	// 오브젝트의 충돌 채널은 WorldStatic, 모든 충돌 채널에 대한 반응은 `Block`.
	circleComponent->bSimulatePhysics = false;				// 움직임에 물리를 적용합니다.
	circleComponent->bApplyImpulseOnDamage = true;	// 데미지를 받을 때 충격을 가합니다.
	circleComponent->bGenerateOverlapEvent = false;	// Overlap 이벤트를 발생시킵니다.
	circleComponent;	// 게임 오브젝트의 루트 컴포넌트가 충돌체 입니다.
	bm->AddChild(circleComponent);

	DamageType radiaDamageType{
		.damageImpulse = 10000.f,
	};
	BossChaseCircleDamageEvent.SetDamageType(radiaDamageType);
	BossChaseCircleDamageEvent.origin = GetLocation();
	BossChaseCircleDamageEvent.radialDamageInfo.maxDamage = damage;
	BossChaseCircleDamageEvent.radialDamageInfo.minDamage = damage;
	BossChaseCircleDamageEvent.componentHits.resize(1);

	for (int i = 0; i < disfellCommand.size(); i++)
		std::cout << (int)disfellCommand[i] + 1 << ", ";

	speedTween = new DotTween<float>(&speedVarias, EasingEffect::InQuad, StepAnimation::StepOnceForward);
	speedTween->SetDuration(5.f);
	speedTween->SetStartPoint(-50.f);
	speedTween->SetEndPoint(100.f);

	scaleTween = new DotTween<float>(&scaleVarias, EasingEffect::InBack, StepAnimation::StepLoopPingPong);
	scaleTween->SetDuration(2.f);
	scaleTween->SetStartPoint(0.9f);
	scaleTween->SetEndPoint(1.1f);
}

void BossChaseCircle::BeginPlay()
{
	__super::BeginPlay();
	bm->SetSprite(L"TestResource/Boss/MagicCircle/BossChaseCircle.png");
	circleComponent->InitCircleRadius(bm->GetSpriteHeight() / 2);	// 반지름이 62이고 높이가 110 인 캡슐 충돌체를 초기화 합니다.
	circleComponent->SetStatus(EObjectStatus::OS_INACTIVE);

	player = GetWorld()->FindActorByType<Player>();
}

void BossChaseCircle::Update(float _dt)
{
	__super::Update(_dt);
	skillDuration -= _dt;
	if (skillDuration > 0.f)
	{
		Math::Vector2 payerPos = player->GetLocation() - GetLocation();
		if (payerPos.Length() < 30.f)
			return;
		payerPos.Normalize();
		speedTween->Update(_dt);
		mv->SetDirection(payerPos);
		mv->SetSpeed(speed + speedVarias);

		scaleTween->Update(_dt);
		bm->SetScale(scaleVarias, scaleVarias);

	}
	if (skillDuration < 0.f)
	{
		BossChaseCircleDamageEvent.radialDamageInfo.innerRadius = 500.f;
		BossChaseCircleDamageEvent.radialDamageInfo.outerRadius = 500.f;
		circleComponent->SetStatus(EObjectStatus::OS_ACTIVE);
		BossChaseCircleDamageEvent.componentHits[0].hitComponent = (PrimitiveComponent*)player->rootComponent;
		player->TakeDamage(damage, BossChaseCircleDamageEvent, nullptr, this);
		EventBus::GetInstance().PushEvent<DisFellEvent>(this, true);
		EventBus::GetInstance().DispatchEvent<DisFellEvent>();
		Destroy();
	}
}

void BossChaseCircle::DisfellAction()
{
	Destroy();
}

void BossChaseCircle::DisfellFailAction()
{
	disfellCommand.clear();
	dissfellindex = 0;
	CreateDisfellCommand();
}

void BossChaseCircle::OnClicked()
{
	__super::OnClicked();
	EventBus::GetInstance().PushEvent<DisFellEvent>(this, false);
	EventBus::GetInstance().DispatchEvent<DisFellEvent>();
}

void BossChaseCircle::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PatternData/Pattern05_Actor.txt");
	int typeInt;
	reflectionResource->ParsingFile(0, isFragile, typeInt, dispelTime, skillDuration, damage, speed, duration);
	type = static_cast<BossSkillType>(typeInt);
}

void BossChaseCircle::ReflectionOut()
{
}
