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
	abm->AddChild(mv);

	circleComponent = CreateComponent<CircleComponent>();
	circleComponent->collisionProperty = CollisionProperty(CollisionPropertyPreset::OverlapAll);	// 오브젝트의 충돌 채널은 WorldStatic, 모든 충돌 채널에 대한 반응은 `Block`.
	circleComponent->bSimulatePhysics = false;				// 움직임에 물리를 적용합니다.
	circleComponent->bApplyImpulseOnDamage = true;	// 데미지를 받을 때 충격을 가합니다.
	circleComponent->bGenerateOverlapEvent = true;	// Overlap 이벤트를 발생시킵니다.
	circleComponent->SetCollisionObjectType(ECollisionChannel::EnemyProjectile);
	circleComponent->collisionProperty.responseContainer.SetAllChannels(CollisionResponse::Ignore);
	circleComponent->collisionProperty.SetCollisionResponse(ECollisionChannel::Player, CollisionResponse::Block);
	circleComponent->collisionProperty.SetCollisionResponse(ECollisionChannel::PlayerProjectile, CollisionResponse::Block);
	
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

	player = GetWorld()->FindActorByType<Player>();
}

void BossThroughProjectile::FixedUpdate(float _fixedRate)
{
	__super::FixedUpdate(_fixedRate);

	// 콜리션 이벤트 뿌렸으니 사망 마크.
	if (destroyThis)
	{
		Destroy();
		return;
	}
}

void BossThroughProjectile::Update(float _dt)
{
	__super::Update(_dt);

	if (destroyThis) return;

	skillDuration -= _dt;
	speedTween->Update(_dt);
	mv->SetSpeed(speedVarias);
	circleComponent->bShouldOverlapTest = true;
	if (skillDuration < 0.f)
	{
		// 다음 FixedUpdate 틱 마지막에 오브젝트 삭제하게 만듦.
		
		// 콜리션 꺼버림
		circleComponent->SetCollisionEnabled(CollisionEnabled::NoCollision);
		circleComponent->bGenerateOverlapEvent = false;

		// 월드에서 직접 콜리션 삭제해서
		// 다음 돌아오는 FixedUpdate 틱에서 OnEndCollision 이벤트 뿌리고
		// 사망하게 해야됨.
		GetWorld()->UnregisterComponentCollision(circleComponent);
		destroyThis = true;
	}
}

void BossThroughProjectile::OnBeginOverlap(Actor* other, const OverlapInfo& overlap)
{
	other->TakeDamage(
		damage,
		BossThroughProjectileDamageEvent,
		nullptr,
		this
	);

	circleComponent->SetCollisionEnabled(CollisionEnabled::NoCollision);
	circleComponent->bGenerateOverlapEvent = false;
	abm->Quit();	// 애니메이션도 종료
	abm->isVisible = false;	// 애니메이션 안보이게 하기
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
	LookAt(_direction);
}
