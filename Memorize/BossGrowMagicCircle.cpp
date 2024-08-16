#include "BossGrowMagicCircle.h" 
#include "../D2DGameEngine/ReflectionResource.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "../D2DGameEngine/ClickComponent.h"
#include "../D2DGameEngine/CircleComponent.h"
#include "../D2DGameEngine/EventBus.h"

#include "D2DGameEngine/World.h"
#include "Player.h"

BossGrowMagicCircle::BossGrowMagicCircle(World* _world)
	:BossSkillActor(_world)
{
	ReflectionIn();
	disfellCommandCount = 4;
	CreateDisfellCommand();
	CreateComponent<ClickComponent>();

	circleComponent = CreateComponent<CircleComponent>();

	circleComponent->collisionProperty = CollisionProperty(CollisionPropertyPreset::OverlapAll);	// 오브젝트의 충돌 채널은 WorldStatic, 모든 충돌 채널에 대한 반응은 `Block`.
	circleComponent->bSimulatePhysics = false;				// 움직임에 물리를 적용합니다.
	circleComponent->bApplyImpulseOnDamage = true;	// 데미지를 받을 때 충격을 가합니다.
	circleComponent->bGenerateOverlapEvent = false;	// Overlap 이벤트를 발생시킵니다.
	circleComponent;	// 게임 오브젝트의 루트 컴포넌트가 충돌체 입니다.
	bm->AddChild(circleComponent);

	DamageType radiaDamageType{
		.damageImpulse = 10000.f,
	};
	BossGrowMagicCircleDamageEvent.SetDamageType(radiaDamageType);
	BossGrowMagicCircleDamageEvent.origin = GetLocation();
	BossGrowMagicCircleDamageEvent.radialDamageInfo.maxDamage = damage;
	BossGrowMagicCircleDamageEvent.radialDamageInfo.minDamage = damage;
	BossGrowMagicCircleDamageEvent.componentHits.resize(1);

	for(int i = 0; i < disfellCommand.size(); i++)
		std::cout << (int)disfellCommand[i] + 1 << ", ";
}

void BossGrowMagicCircle::BeginPlay()
{
	__super::BeginPlay();
	bm->SetSprite(L"TestResource/Boss/MagicCircle/Pattern06_MagicCircle.png");
	circleComponent->InitCircleRadius(bm->GetSpriteHeight() / 2);	// 반지름이 62이고 높이가 110 인 캡슐 충돌체를 초기화 합니다.
	circleComponent->SetStatus(EObjectStatus::OS_INACTIVE);

	player = GetWorld()->FindActorByType<Player>();
}

void BossGrowMagicCircle::Update(float _dt)
{
	__super::Update(_dt);
	if (skillDuration > 0.f)
	{
		bm->Scale(1.0f + _dt * 0.1f, 1.0f + _dt * 0.1f);
		skillDuration -= _dt;
	}
	if (skillDuration < 0.f)
	{
		BossGrowMagicCircleDamageEvent.radialDamageInfo.innerRadius = 500.f;
		BossGrowMagicCircleDamageEvent.radialDamageInfo.outerRadius = 500.f;
		circleComponent->SetStatus(EObjectStatus::OS_ACTIVE);
		BossGrowMagicCircleDamageEvent.componentHits[0].hitComponent = (PrimitiveComponent*)player->rootComponent;
		player->TakeDamage(damage, BossGrowMagicCircleDamageEvent, nullptr, this);
		EventBus::GetInstance().PushEvent<DisFellEvent>(this, true);
		EventBus::GetInstance().DispatchEvent<DisFellEvent>();
		SetStatus(EObjectStatus::OS_DESTROY);
	}
}

void BossGrowMagicCircle::DisfellOneCountAction()
{
	bm->Scale(0.9f, 0.9f);
}

void BossGrowMagicCircle::DisfellAction()
{
	SetStatus(EObjectStatus::OS_DESTROY);
}

void BossGrowMagicCircle::OnClicked()
{
	__super::OnClicked();
	EventBus::GetInstance().PushEvent<DisFellEvent>(this, false);
	EventBus::GetInstance().DispatchEvent<DisFellEvent>();
}

void BossGrowMagicCircle::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PatternData/Pattern06_Actor.txt");
	int typeInt;
	reflectionResource->ParsingFile(0, isFragile, typeInt, dispelTime, skillDuration, damage);
	type = static_cast<BossSkillType>(typeInt);
}

void BossGrowMagicCircle::ReflectionOut() {}
