#include "BossProjectile.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/AnimationBitmapComponent.h"
#include "../D2DGameEngine/CircleComponent.h"
#include "MovementComponent.h"
#include "D2DGameEngine/World.h"
#include "Player.h"

#include "../D2DGameEngine/AnimationEffect.h"

BossProjectile::BossProjectile(World* _world)
	:BossSkillActor(_world)
{
	bm->isVisible = false;
	abm = CreateComponent<AnimationBitmapComponent>();
	rootComponent = abm;
	abm->MarkBoundsDirty();
	mv = CreateComponent<MovementComponent>();
	rootComponent->AddChild(mv);

}

void BossProjectile::BeginPlay()
{
	__super::BeginPlay();

	{
		abm->SetSprite(L"TestResource/Boss/Projectile/Boss_Projectile.png");
		abm->SliceSpriteSheet(137, 254, 0, 0, 0, 0);
		abm->SetFrameDurations({ 0.03f });
		abm->SetLoop(true);
		abm->Trigger(true);
	}

	circleComponent->InitCircleRadius(150 / 2);

	player = GetWorld()->FindActorByType<Player>();
}

void BossProjectile::Update(float _dt)
{
	__super::Update(_dt);
	duration -= _dt;
	if (duration < 0.f)
	{
		circleComponent->SetCollisionEnabled(CollisionEnabled::NoCollision);
		circleComponent->bGenerateOverlapEvent = false;
		Destroy();
	}
}

bool BossProjectile::Destroy()
{
	//BlickSource Effect
	{
		AnimationEffect* DestoryProjectileEffect = GetWorld()->GetEffectSystem().CreateEffect<AnimationEffect>();
		DestoryProjectileEffect->SetSprite(L"TestResource/Boss/Projectile/Boss_ProjectileEffect.png");
		DestoryProjectileEffect->GetAnimationBitmapComponent()->SliceSpriteSheet(512, 512, 0, 0, 0, 0);
		DestoryProjectileEffect->GetAnimationBitmapComponent()->SetFrameDurations({ 0.025f });
		DestoryProjectileEffect->GetAnimationBitmapComponent()->Trigger(true);
		DestoryProjectileEffect->SetAliveTime(1.f);
		auto Pos = GetLocation();
		DestoryProjectileEffect->SetLocation(Pos.x, Pos.y);
	}

	return __super::Destroy();
}

void BossProjectile::SetActor(std::wstring _fileName)
{
	reflectionResource = ResourceManager::LoadResource<ReflectionResource>(_fileName);
	ReflectionIn();

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
	BossProjectileDamageEvent.SetDamageType(radiaDamageType);
	BossProjectileDamageEvent.origin = GetLocation();
	BossProjectileDamageEvent.radialDamageInfo.maxDamage = damage;
	BossProjectileDamageEvent.radialDamageInfo.minDamage = damage;
	BossProjectileDamageEvent.componentHits.resize(1);
}

void BossProjectile::ReflectionIn()
{
	reflectionResource->ParsingFile(0, 
		isFragile,
		dispelTime, 
		skillDuration, 
		damage,
		speed,
		duration);
}

void BossProjectile::ReflectionOut()
{
}

void BossProjectile::SetPosAndDerection(Math::Vector2 _startPos, Math::Vector2 _direction)
{
	SetLocation(_startPos.x, _startPos.y);
	mv->SetDirection(_direction);
	mv->SetSpeed(speed);

	Math::Vector2 w = _direction;
	Math::Vector2 v = Up();

	float x = w.y * v.x - w.x * v.y;
	float y = w.x * v.x + w.y * v.y;
	float rad = atan2(x, y);

	Rotate(Math::RadianToDegree(rad));
}

void BossProjectile::OnBeginOverlap(Actor* other, const OverlapInfo& overlap)
{
	other->TakeDamage(
		damage,
		BossProjectileDamageEvent,
		nullptr,
		this
	);
	circleComponent->SetCollisionEnabled(CollisionEnabled::NoCollision);
	circleComponent->bGenerateOverlapEvent = false;

	Destroy();
}