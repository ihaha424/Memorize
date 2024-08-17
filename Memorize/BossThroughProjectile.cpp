#include "BossThroughProjectile.h"
#include "../D2DGameEngine/ReflectionResource.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/AnimationBitmapComponent.h"
#include "MovementComponent.h"
#include "../D2DGameEngine/ClickComponent.h"
#include "../D2DGameEngine/CircleComponent.h"
#include "../D2DGameEngine/EventBus.h"

#include "D2DGameEngine/World.h"
#include "Player.h"

BossThroughProjectile::BossThroughProjectile(World* _world)
	:BossSkillActor(_world)
{
	ReflectionIn();

	bm->isVisible = false;
	abm = CreateComponent<AnimationBitmapComponent>();
	rootComponent = abm;
	abm->MarkBoundsDirty();
	mv = CreateComponent<MovementComponent>();
	rootComponent->AddChild(mv);
	abm->AddChild(mv);

	circleComponent = CreateComponent<CircleComponent>();

	circleComponent->collisionProperty = CollisionProperty(CollisionPropertyPreset::OverlapAll);	// 오브젝트의 충돌 채널은 WorldStatic, 모든 충돌 채널에 대한 반응은 `Block`.
	circleComponent->bSimulatePhysics = false;				// 움직임에 물리를 적용합니다.
	circleComponent->bApplyImpulseOnDamage = true;	// 데미지를 받을 때 충격을 가합니다.
	circleComponent->bGenerateOverlapEvent = false;	// Overlap 이벤트를 발생시킵니다.
	circleComponent;	// 게임 오브젝트의 루트 컴포넌트가 충돌체 입니다.
	abm->AddChild(circleComponent);

	DamageType radiaDamageType{
	.damageImpulse = 10000.f,
	};
	BossThroughProjectileDamageEvent.SetDamageType(radiaDamageType);
	BossThroughProjectileDamageEvent.origin = GetLocation();
	BossThroughProjectileDamageEvent.radialDamageInfo.maxDamage = damage;
	BossThroughProjectileDamageEvent.radialDamageInfo.minDamage = damage;
	BossThroughProjectileDamageEvent.componentHits.resize(1);

	speedTween = new DotTween<float>(&speedVarias, EasingEffect::InExpo, StepAnimation::StepOnceForward);
	speedTween->SetDuration(0.5f);
	speedTween->SetStartPoint(0.f);
	speedTween->SetEndPoint(speed);
}

void BossThroughProjectile::BeginPlay()
{
	__super::BeginPlay();
	
	{
		abm->SetSprite(L"TestResource/Boss/MagicCircle/BossThroughProjectile.png");
		abm->SliceSpriteSheet(140, 254, 0, 0, 0, 0);
		abm->FrameResize(73);
		abm->SetFrameDurations({ 0.05f });
		abm->SetLoop(true);
		abm->Trigger(true);
	}

	circleComponent->InitCircleRadius(150 / 2);	// 반지름이 62이고 높이가 110 인 캡슐 충돌체를 초기화 합니다.
	circleComponent->SetStatus(EObjectStatus::OS_INACTIVE);

	player = GetWorld()->FindActorByType<Player>();
}

void BossThroughProjectile::Update(float _dt)
{
	__super::Update(_dt);

	skillDuration -= _dt;
	speedTween->Update(_dt);
	mv->SetSpeed(speedVarias);
	if (skillDuration < 0.f)
	{
		BossThroughProjectileDamageEvent.radialDamageInfo.innerRadius = 500.f;
		BossThroughProjectileDamageEvent.radialDamageInfo.outerRadius = 500.f;
		circleComponent->SetStatus(EObjectStatus::OS_ACTIVE);
		BossThroughProjectileDamageEvent.componentHits[0].hitComponent = (PrimitiveComponent*)player->rootComponent;
		player->TakeDamage(damage, BossThroughProjectileDamageEvent, nullptr, this);
		SetStatus(EObjectStatus::OS_DESTROY);
	}
}


void BossThroughProjectile::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PatternData/Pattern09_Actor.txt");
	reflectionResource->ParsingFile(0, dispelTime, skillDuration, damage, speed, duration);
}

void BossThroughProjectile::ReflectionOut()
{}

void BossThroughProjectile::SetPosAndDerection(Math::Vector2 _startPos, Math::Vector2 _direction)
{
	SetLocation(_startPos.x, _startPos.y);
	mv->SetDirection(_direction);
	RotateToward(_direction);
}
