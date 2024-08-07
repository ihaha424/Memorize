#include "FireballProjectile.h"
#include "../D2DGameEngine/BitmapComponent.h"

FireballProjectile::FireballProjectile(World* _world) : Projectile(_world)
{
	SetTickProperties(TICK_UPDATE | TICK_RENDER);

	BitmapComponent* bm = GetComponent<BitmapComponent>();
	bm->SetSprite(L"Resources/fireball.png");
}

FireballProjectile::~FireballProjectile()
{

}