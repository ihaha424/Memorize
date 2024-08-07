#include "Projectile.h"
#include "MovementComponent.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "../D2DGameEngine/Mouse.h"

Projectile::Projectile(World* _world) : Actor(_world)
{
	CreateComponent<BitmapComponent>();
	CreateComponent<MovementComponent>();
}

Projectile::~Projectile()
{
}

void Projectile::Move(Math::Vector2 _direction, float _speed)
{
	MovementComponent* mv = GetComponent<MovementComponent>();
	mv->SetDirection(_direction);
	mv->SetSpeed(_speed);
}
