#include "FireballProjectile.h"
#include "../D2DGameEngine/BitmapComponent.h"

FireballProjectile::FireballProjectile(World* _world) : Projectile(_world)
{
	bm->SetSprite(L"TestResource/fireball.png");
}

FireballProjectile::~FireballProjectile()
{
}

void FireballProjectile::OnBeginOverlap(Actor* other)
{
	__super::OnBeginOverlap(other);

	//TODO : Æø¹ß
	Inactivate();
}
