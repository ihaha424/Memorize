#include "BossThroughProjectile.h"
#include "../D2DGameEngine/ReflectionResource.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/AnimationBitmapComponent.h"
#include "MovementComponent.h"
#include "../D2DGameEngine/ClickComponent.h"
#include "../D2DGameEngine/CircleComponent.h"
#include "../D2DGameEngine/EventBus.h"

#include "D2DGameEngine/World.h"
#include "../D2DGameEngine/AnimationEffect.h"
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
	circleComponent->collisionProperty = CollisionProperty(CollisionPropertyPreset::EnemyProjectile);	// 오브젝트의 충돌 채널은 WorldStatic, 모든 충돌 채널에 대한 반응은 `Block`.
	circleComponent->bSimulatePhysics = false;				// 움직임에 물리를 적용합니다.
	circleComponent->bApplyImpulseOnDamage = true;	// 데미지를 받을 때 충격을 가합니다.
	circleComponent->bGenerateOverlapEvent = true;	// Overlap 이벤트를 발생시킵니다.
	circleComponent->SetCollisionObjectType(ECollisionChannel::EnemyProjectile);
	circleComponent->collisionProperty.responseContainer.SetAllChannels(CollisionResponse::Ignore);
	circleComponent->collisionProperty.SetCollisionResponse(ECollisionChannel::Player, CollisionResponse::Block);
	circleComponent->collisionProperty.SetCollisionResponse(ECollisionChannel::PlayerProjectile, CollisionResponse::Block);
	circleComponent->bShouldOverlapTest = true;

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
		abm->SetSprite(L"TestResource/Boss/ThroughProjectileEffect/Boss_ThroughProjectile.png");
		abm->SliceSpriteSheet(240, 181, 0, 0, 0, 0);
		abm->FrameResize(120);
		abm->SetFrameDurations({ 0.016f });
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

	if (skillDuration < 0.f)
	{
		// 다음 FixedUpdate 틱 마지막에 오브젝트 삭제하게 만듦.
		
		// 콜리션 꺼버림
		circleComponent->SetCollisionEnabled(CollisionEnabled::NoCollision);
		circleComponent->bGenerateOverlapEvent = false;
		Destroy();
	}
}

bool BossThroughProjectile::Destroy()
{
	//BlickSource Effect
	{
		AnimationEffect* DestoryProjectileEffect = GetWorld()->GetEffectSystem().CreateEffect<AnimationEffect>();
		DestoryProjectileEffect->SetSprite(L"TestResource/Boss/ThroughProjectileEffect/Boss_ThroughProjectileEffect.png");
		DestoryProjectileEffect->GetAnimationBitmapComponent()->SliceSpriteSheet(300, 300, 0, 0, 0, 0);
		DestoryProjectileEffect->GetAnimationBitmapComponent()->SetFrameDurations({ 0.025f });
		DestoryProjectileEffect->GetAnimationBitmapComponent()->Trigger(true);
		DestoryProjectileEffect->SetAliveTime(1.f);
		auto Pos = GetLocation();
		DestoryProjectileEffect->SetLocation(Pos.x, Pos.y);
	}


	return __super::Destroy();
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

	Destroy();
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
	
	Math::Vector2 w = _direction;
	Math::Vector2 v = -Right();

	float x = w.y * v.x - w.x * v.y;
	float y = w.x * v.x + w.y * v.y;
	float rad = atan2(x, y);

	Rotate(Math::RadianToDegree(rad));
}
