#include "PrismReflectionProjectile.h"
#include "D2DGameEngine/BitmapComponent.h"

PrismReflectionProjectile::PrismReflectionProjectile(World* _world) : Projectile(_world)
{
	SetTickProperties(TICK_UPDATE | TICK_RENDER);

	BitmapComponent* bm = GetComponent<BitmapComponent>();
	bm->SetSprite(L"TestResource/Skill/Projectile/PrismReflection.png");
}

PrismReflectionProjectile::~PrismReflectionProjectile()
{
}
