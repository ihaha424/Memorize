#include "Projectile.h"
#include "MovementComponent.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "../D2DGameEngine/Mouse.h"
#include "../D2DGameEngine/Character.h"
#include "D2DGameEngine/BoxComponent.h"
#include "D2DGameEngine/DamageEvent.h"
#include "Boss.h"

Projectile::Projectile(World* _world) : Actor(_world)
{
	SetTickProperties(TICK_PHYSICS | TICK_UPDATE | TICK_RENDER);

	rootComponent = bm = CreateComponent<BitmapComponent>();
	mv = CreateComponent<MovementComponent>();
	rootComponent->AddChild(mv);

	box = CreateComponent<BoxComponent>();
	box->collisionProperty = CollisionProperty(CollisionPropertyPreset::BlockAll);
	box->bSimulatePhysics = false;	// 움직임에 물리를 적용하지 않습니다.
	box->bApplyImpulseOnDamage = false;	// 데미지를 받을 때 충격을 가합니다.
	box->bGenerateOverlapEvent = true;	// Overlap 이벤트를 발생시킵니다.
	rootComponent->AddChild(box);

	mv->SetStatus(OS_INACTIVE);
	bm->SetStatus(OS_INACTIVE);
	box->SetStatus(OS_INACTIVE);
}

Projectile::~Projectile()
{
}

void Projectile::OnBeginOverlap(Actor* other)
{
	__super::OnBeginOverlap(other);

	Boss* boss = dynamic_cast<Boss*>(other);
	if (boss == nullptr) return;

	//대미지를 입힘
	PointDamageEvent damageEvent;
	damageEvent.damage = damage;
	damageEvent.shotDirection = -mv->GetDirection();

	boss->TakeDamage(damage, damageEvent, boss->GetController(), this);

}


void Projectile::SetVelocity(Math::Vector2 _direction, float _speed)
{ 
	mv->SetDirection(_direction);
	mv->SetSpeed(_speed);
}

void Projectile::BeginPlay()
{
	__super::BeginPlay();

	box->InitBoxExtent({ bm->GetSprite()->GetResource()->GetSize().width, bm->GetSprite()->GetResource()->GetSize().height });
}

void Projectile::Update(float _dt)
{
	__super::Update(_dt);

	elapsedTime += _dt;

	if (elapsedTime > delay)
	{
		
		mv->SetStatus(OS_ACTIVE);
		bm->SetStatus(OS_ACTIVE);
		box->SetStatus(OS_ACTIVE);
	}

	if (elapsedTime > duration)
	{
		elapsedTime = 0.f;
		mv->SetStatus(OS_INACTIVE);
		bm->SetStatus(OS_INACTIVE);
		box->SetStatus(OS_INACTIVE);
		Inactivate();
	}
}
