#include "FireballProjectile.h"
#include "../D2DGameEngine/BitmapComponent.h"

FireballProjectile::FireballProjectile(World* _world) : Projectile(_world)
{
	BitmapComponent* bm = GetComponent<BitmapComponent>();
	bm->SetSprite(L"TestResource/fireball.png");
}

FireballProjectile::~FireballProjectile()
{
}