#include "BossChasingBomb.h"

#include "D2DGameEngine/World.h"
#include "D2DGameEngine/ReflectionResource.h"
#include "D2DGameEngine/ResourceManager.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "D2DGameEngine/CircleComponent.h"

#include "Player.h"

ChasingBomb::ChasingBomb(World* _world) : BossSkillActor(_world)
{
	renderLayer = 2;
	bm->isVisible = false;

	isDispel = false;
	isFragile = true;
	type = Projectile;
	dispelTime = 0.f;
	skillDuration = 3.f;
	// std::max(DispelTime - SkillDuration, 0);
	damage = 20.f;
	speed = 400.f;
	duration = 5.f;

	bm->maxSpeed = speed;

	// Bomb trigger setting
	bombTrigger = CreateComponent<CircleComponent>();
	bm->AddChild(bombTrigger);
	bombTrigger->InitCircleRadius(50);
	bombTrigger->collisionProperty = CollisionProperty(CollisionPropertyPreset::EnemyProjectile);
	bombTrigger->bApplyImpulseOnDamage = false;
	bombTrigger->bGenerateOverlapEvent = true;

	chasingBomb = CreateComponent<AnimationBitmapComponent>();
	bombTrigger->AddChild(chasingBomb);
	chasingBomb->SetSprite(L"TestResource/Boss/ChasingBomb/Boss_ChaseProjectile.png");
	chasingBomb->SliceSpriteSheet(140, 254, 0, 0, 0, 0);
	chasingBomb->FrameResize(61);
	chasingBomb->SetFrameDurations({ 0.01f });
	chasingBomb->SetLoop(true);
	chasingBomb->Trigger(true);
	chasingBomb->SetRotation(-90);
	chasingBomb->Translate(-50, 0);

	// Explosion setting
	bombExplosionRadius = CreateComponent<CircleComponent>();
	bm->AddChild(bombExplosionRadius);
	bombExplosionRadius->InitCircleRadius(75);
	bombExplosionRadius->collisionProperty = CollisionProperty(CollisionPropertyPreset::EnemyPattern);
	bombExplosionRadius->SetCollisionEnabled(CollisionEnabled::NoCollision);
	bombExplosionRadius->bApplyImpulseOnDamage = true;
	bombExplosionRadius->bGenerateOverlapEvent = false;	// True

	explosionEffect = CreateComponent<AnimationBitmapComponent>();
	bombExplosionRadius->AddChild(explosionEffect);
	explosionEffect->SetSprite(L"TestResource/Boss/ChasingBomb/Boss_ChaseProjectileEffect.png");
	explosionEffect->SliceSpriteSheet(150, 150, 0, 0, 0, 0);
	explosionEffect->FrameResize(7);
	explosionEffect->SetFrameDurations({ 0.01f });
	explosionEffect->Trigger(false);
	explosionEffect->isVisible = false;

	// Damage setting
	DamageType damageType{
		.damageImpulse = 10000.f,
	};
	bombDamageEvent.SetDamageType(damageType);
	bombDamageEvent.damage = damage;
}

void ChasingBomb::BeginPlay()
{
	Super::BeginPlay();
	player = GetWorld()->FindActorByType<Player>();

	Math::Vector2 playerLocation = player->GetLocation();
	Math::Vector2 toPlayer = playerLocation - GetLocation();
	toPlayer.Normalize();

	SetVelocity(toPlayer * speed);
	LookAt(toPlayer);
}

void ChasingBomb::FixedUpdate(float _fixedRate)
{
	Super::FixedUpdate(_fixedRate);

	// 콜리션 이벤트 뿌렸으니 사망 마크.
	if (destroyThis)
	{
		Destroy();
		return;
	}
}

void ChasingBomb::Update(float _dt)
{
	Super::Update(_dt);

	if (destroyThis) return;

	if (!exploding)	// Chase player
	{
		// 플레이어 쫓아가게 하기.
		Math::Vector2 playerLocation = player->GetLocation();
		Math::Vector2 toPlayer = playerLocation - GetLocation();
		toPlayer.Normalize();
		// 플레이어를 향해 조금씩 전진함.
		AddVelocity(toPlayer * speed * 0.01f);

		// 현재의 속력 방향을 향하게 방향 틀기.
		Math::Vector2 lookAtDirection = GetVelocity();
		lookAtDirection.Normalize();
		RotateToward(lookAtDirection);

		skillDuration -= _dt;
		if (skillDuration < 0.f)	// 스킬 종료
		{
			// 폭팔 트리거 작동했으니 끄고
			bombTrigger->SetCollisionEnabled(CollisionEnabled::NoCollision);
			bombTrigger->bGenerateOverlapEvent = false;
			chasingBomb->Quit();	// 애니메이션도 종료
			chasingBomb->isVisible = false;	// 애니메이션 안보이게 하기

			// 폭발 범위를 콜리션으로 전환
			bombExplosionRadius->SetCollisionEnabled(CollisionEnabled::EnableCollision);
			bombExplosionRadius->bGenerateOverlapEvent = true;
			explosionEffect->Trigger(true);	// 폭발 애니메이션 트리거
			explosionEffect->isVisible = true;	// 애니메이션 보이게 하기

			exploding = true;	// 폭발 중 마크
		} 
		else
		{
			bombTrigger->bShouldOverlapTest = true;
		}
	}
	else
	{
		explodingTime -= _dt;
		if (explodingTime < 0.f)	// 폭발 종료
		{
			DistroyThis();
		}
		else
		{
			bombExplosionRadius->bShouldOverlapTest = true;
		}
	}
}

bool ChasingBomb::Destroy()
{
	return __super::Destroy();
}

void ChasingBomb::DisfellAction()
{
}

void ChasingBomb::OnBeginOverlap(Actor* other, const OverlapInfo& overlap)
{
	OBJ_MESSAGE("Overlapped!");
	if (!exploding)	// Projectile mode
	{
		// 플레이어랑 닿으면 폭발
		// 폭팔 트리거 작동했으니 끄고
		bombTrigger->SetCollisionEnabled(CollisionEnabled::NoCollision);
		bombTrigger->bGenerateOverlapEvent = false;
		chasingBomb->Quit();	// 애니메이션도 종료
		chasingBomb->isVisible = false;	// 애니메이션 안보이게 하기

		// 폭발 범위를 콜리션으로 전환
		bombExplosionRadius->SetCollisionEnabled(CollisionEnabled::EnableCollision);
		bombExplosionRadius->bGenerateOverlapEvent = true;
		explosionEffect->Trigger(true);	// 폭발 애니메이션 트리거
		explosionEffect->isVisible = true;	// 애니메이션 보이게 하기

		exploding = true;	// 폭발 중 마크
	}
	else	// Explosion mode
	{
		// 플레이어랑 닿으면 데미지 이벤트
		Math::Vector2 playerLocation = player->GetLocation();
		Math::Vector2 toPlayer = playerLocation - GetLocation();
		toPlayer.Normalize();
		bombDamageEvent.shotDirection = toPlayer;
		bombDamageEvent.hitInfo.hitComponent = (PrimitiveComponent*) player->rootComponent;
		other->TakeDamage(damage, bombDamageEvent, nullptr, this);
	}
}

void ChasingBomb::OnEndOverlap(Actor* other, const OverlapInfo& overlap)
{
}

void ChasingBomb::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PatternData/Pattern05_Actor.txt");
	int typeInt;
	reflectionResource->ParsingFile(0, isFragile, typeInt, dispelTime, skillDuration, damage);
	type = static_cast<BossSkillType>(typeInt);
}

void ChasingBomb::ReflectionOut()
{
}

void ChasingBomb::DistroyThis()
{
	// 콜리션 꺼버림
	bombExplosionRadius->SetCollisionEnabled(CollisionEnabled::NoCollision);
	bombExplosionRadius->bGenerateOverlapEvent = false;

	// 월드에서 직접 콜리션 삭제해서
	// 다음 돌아오는 FixedUpdate 틱에서 OnEndCollision 이벤트 뿌리고
	// 사망하게 해야됨.
	GetWorld()->UnregisterComponentCollision(bombTrigger);
	GetWorld()->UnregisterComponentCollision(bombExplosionRadius);

	// 다음 FixedUpdate 틱 마지막에 오브젝트 삭제하게 만듦.
	destroyThis = true;
}
