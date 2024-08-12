#include "FireballProjectile.h"
#include "../D2DGameEngine/BitmapComponent.h"

FireballProjectile::FireballProjectile(World* _world) : Projectile(_world)
{
	bm->SetSprite(L"TestResource/fireball.png");
}

FireballProjectile::~FireballProjectile()
{
}