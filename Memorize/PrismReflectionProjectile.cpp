#include "PrismReflectionProjectile.h"
#include "D2DGameEngine/BitmapComponent.h"

PrismReflectionProjectile::PrismReflectionProjectile(World* _world) : Projectile(_world)
{
	bm->SetSprite(L"TestResource/Skill/Projectile/PrismReflection.png");
}

PrismReflectionProjectile::~PrismReflectionProjectile()
{
}

void PrismReflectionProjectile::Update(float _dt)
{
	__super::Update(_dt);
}
