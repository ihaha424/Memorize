#include "Projectile.h"
#include "MovementComponent.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "../D2DGameEngine/Mouse.h"
#include "../D2DGameEngine/Character.h"
#include "D2DGameEngine/BoxComponent.h"

Projectile::Projectile(World* _world) : Actor(_world)
{
	SetTickProperties(TICK_PHYSICS | TICK_UPDATE | TICK_RENDER);

	rootComponent = bm = CreateComponent<BitmapComponent>();
	mv = CreateComponent<MovementComponent>();
	rootComponent->AddChild(mv);

	mv->SetStatus(OS_INACTIVE);
	bm->SetStatus(OS_INACTIVE);
}

Projectile::~Projectile()
{
}

void Projectile::SetVelocity(Math::Vector2 _direction, float _speed)
{ 
	mv->SetDirection(_direction);
	mv->SetSpeed(_speed);
}

void Projectile::Update(float _dt)
{
	__super::Update(_dt);

	elapsedTime += _dt;

	if (elapsedTime > delay)
	{
		mv->SetStatus(OS_ACTIVE);
		bm->SetStatus(OS_ACTIVE);
	}
}
