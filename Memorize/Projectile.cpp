#include "Projectile.h"
#include "MovementComponent.h"
#include "../D2DGameEngine/Animator.h"
#include "../D2DGameEngine/AnimationState.h"
#include "../D2DGameEngine/Mouse.h"
#include "../D2DGameEngine/Character.h"
#include "D2DGameEngine/BoxComponent.h"
#include "D2DGameEngine/DamageEvent.h"
#include "Boss.h"
#include "BossProjectile.h"

Projectile::Projectile(World* _world) : Actor(_world)
{
	SetTickProperties(TICK_PHYSICS | TICK_UPDATE | TICK_RENDER);

	rootComponent = anim = CreateComponent<Animator>();
	mv = CreateComponent<MovementComponent>();
	rootComponent->AddChild(mv);

	box = CreateComponent<BoxComponent>();
	box->collisionProperty = CollisionProperty(CollisionPropertyPreset::OverlapAll);
	box->bSimulatePhysics = false;	// 움직임에 물리를 적용하지 않습니다.
	box->bApplyImpulseOnDamage = false;	// 데미지를 받을 때 충격을 가합니다.
	box->bGenerateOverlapEvent = true;	// Overlap 이벤트를 발생시킵니다.
	rootComponent->AddChild(box);

	mv->SetStatus(OS_INACTIVE);
	anim->SetStatus(OS_INACTIVE);
	box->SetStatus(OS_INACTIVE);

	normalState = anim->CreateState<AnimationState>();
	endingState = anim->CreateState<AnimationState>();
	
}

Projectile::~Projectile()
{
}

void Projectile::OnBeginOverlap(Actor* other, const OverlapInfo& overlap)
{
	__super::OnBeginOverlap(other, overlap);

	if (box->bGenerateOverlapEvent == false)
		return;

	//대미지를 입힘
	DamageEvent damageEvent;
	DamageType damageType{
		.damageImpulse = 10000.f, //충격량 넣어주는 게 맞는지?
	};
	damageEvent.SetDamageType(damageType);

	other->TakeDamage(damage, damageEvent, nullptr, this);

	//투과되지 않는 발세체의 경우 멈추고 폭발 처리
	if (!bIsPassable)
	{
		bEnding = true;
		anim->SetState(endingState);
		mv->SetSpeed(0);
		elapsedTime = duration + delay;
	}

	//다른 공격과 충돌하는 스킬은 충돌 처리
	if (bCollideWithOtherAttack)
	{
		BossProjectile* bossProjectile = dynamic_cast<BossProjectile*>(other);
		if (bossProjectile != nullptr)
		{
			bEnding = true;
			anim->SetState(endingState);
			mv->SetSpeed(0);
		}
	}
}


void Projectile::SetVelocity(Math::Vector2 _direction, float _speed)
{ 
	mv->SetDirection(_direction);
	mv->SetSpeed(_speed);
}

void Projectile::BeginPlay()
{
	__super::BeginPlay();
}

void Projectile::Initialize()
{
	anim->SetState(normalState);
}

void Projectile::FixedUpdate(float _fixedRate)
{
	__super::FixedUpdate(_fixedRate);
}

void Projectile::Update(float _dt)
{
	__super::Update(_dt);

	elapsedTime += _dt;

	//딜레이 시간 
	if (elapsedTime > delay)
	{
		mv->SetStatus(OS_ACTIVE);
		anim->SetStatus(OS_ACTIVE);
		box->SetStatus(OS_ACTIVE);
	}
	//지속시간 이후 끝나기 전까지
	if (bHasEnding && !bEnding && elapsedTime > duration + delay)
	{
		bEnding = true;
		anim->SetState(endingState);
		mv->SetSpeed(0);
	}
	//+터지는 시간
	if (elapsedTime > duration + delay + endingTime)
	{
		elapsedTime = 0.f;
		mv->SetStatus(OS_INACTIVE);
		anim->SetStatus(OS_INACTIVE);
		box->SetStatus(OS_INACTIVE);
		Inactivate();
	}
}
