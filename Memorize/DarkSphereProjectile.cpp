#include "DarkSphereProjectile.h"
#include "D2DGameEngine/BitmapComponent.h"
#include "MovementComponent.h"

DarkSphereProjectile::DarkSphereProjectile(World* _world) : Projectile(_world)
{
	bm->SetSprite(L"TestResource/Skill/Projectile/DarkSphere.png");
}

DarkSphereProjectile::~DarkSphereProjectile()
{
}