#include "Projectile.h"
#include "MovementComponent.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "../D2DGameEngine/Mouse.h"
#include "../D2DGameEngine/Character.h"

Projectile::Projectile(World* _world) : Actor(_world)
{
	rootComponent = CreateComponent<BitmapComponent>();
	rootComponent->AddChild(CreateComponent<MovementComponent>());
}

Projectile::~Projectile()
{
}

void Projectile::SetVelocity(Math::Vector2 _direction, float _speed)
{
	MovementComponent* mv = GetComponent<MovementComponent>();
	mv->SetDirection(_direction);
	mv->SetSpeed(_speed);
}
