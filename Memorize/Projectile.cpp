#include "Projectile.h"
#include "MovementComponent.h"
#include "../D2DGameEngine/Animator.h"
#include "../D2DGameEngine/AnimationState.h"
#include "../D2DGameEngine/Mouse.h"
#include "../D2DGameEngine/Character.h"
#include "D2DGameEngine/PolygonComponent.h"
#include "D2DGameEngine/DamageEvent.h"
#include "Boss.h"
#include "BossProjectile.h"
#include "ProjectileSkill.h"

Projectile::Projectile(World* _world) : SkillActor(_world)
{
	SetTickProperties(TICK_PHYSICS | TICK_UPDATE | TICK_RENDER);

	rootComponent = anim = CreateComponent<Animator>();
	mv = CreateComponent<MovementComponent>();
	rootComponent->AddChild(mv);

	box = CreateComponent<PolygonComponent>();
	box->SetCollisionObjectType(ECollisionChannel::PlayerProjectile);
	box->collisionProperty.SetCollisionResponse(ECollisionChannel::Enemy, CollisionResponse::Overlap);
	box->collisionProperty.SetCollisionResponse(ECollisionChannel::EnemyProjectile, CollisionResponse::Overlap);
	box->bSimulatePhysics = false;	// �����ӿ� ������ �������� �ʽ��ϴ�.
	box->bApplyImpulseOnDamage = false;	// �������� ���� �� ����� ���մϴ�.
	box->bGenerateOverlapEvent = true;	// Overlap �̺�Ʈ�� �߻���ŵ�ϴ�.
	rootComponent->AddChild(box);

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

	//������� ����
	DamageEvent damageEvent;
	DamageType damageType{
		.damageImpulse = 10000.f,
	};
	damageEvent.SetDamageType(damageType);

	other->TakeDamage(damage, damageEvent, nullptr, this);

	
	//�������� �ʴ� �߼�ü�� ��� ���߰� ���� ó��
	if (!bIsPassable && !bEnding)
	{
		bEnding = true;
		if(bHasEnding)
			anim->SetState(endingState);
		mv->SetSpeed(0);
		elapsedTime = duration + delay;
		ownerSkill->bEnd = true;
	}

	//�ٸ� ���ݰ� �浹�ϴ� ��ų�� �浹 ó��
	if (bCollideWithOtherAttack)
	{
		BossProjectile* bossProjectile = dynamic_cast<BossProjectile*>(other);
		if (bossProjectile != nullptr && !bEnding)
		{
			bEnding = true;
			elapsedTime = duration + delay;
			mv->SetSpeed(0);
			ownerSkill->bEnd = true;
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
	mv->SetStatus(OS_INACTIVE);
	anim->SetStatus(OS_INACTIVE);
	box->SetStatus(OS_INACTIVE);
	Inactivate();
}

void Projectile::Initialize()
{
	anim->SetState(normalState);
	elapsedTime = 0.f;
	bEnding = false;
}

void Projectile::FixedUpdate(float _fixedRate)
{
	__super::FixedUpdate(_fixedRate);
}

void Projectile::Update(float _dt)
{
	__super::Update(_dt);

	box->bShouldOverlapTest = true;

	elapsedTime += _dt;

	//������ �ð� 
	if (elapsedTime > delay)
	{
		mv->SetStatus(OS_ACTIVE);
		anim->SetStatus(OS_ACTIVE);
		box->SetStatus(OS_ACTIVE);
	}
	//���ӽð� ���� ������ ������
	if (bHasEnding && !bEnding && elapsedTime > duration + delay)
	{
		bEnding = true;
		anim->SetState(endingState);
		mv->SetSpeed(0);
	}
	//+������ �ð�
	if (elapsedTime > duration + delay + endingTime)
	{
		elapsedTime = 0.f;
		ownerSkill->bEnd = true;
		mv->SetStatus(OS_INACTIVE);
		anim->SetStatus(OS_INACTIVE);
		box->SetStatus(OS_INACTIVE);
		Inactivate();
	}
}
