#include "FireballProjectile.h"
#include "../D2DGameEngine/AnimationState.h"
#include "../D2DGameEngine/Animator.h"

FireballProjectile::FireballProjectile(World* _world) : Projectile(_world)
{
	normalState->SetSprite(L"TestResource/Player/Skill/Skill_DarkSphere01.png");
	normalState->SliceSpriteSheet(140, 254, 0, 0, 0, 0);
	normalState->FrameResize(73);
	normalState->SetFrameDurations({ 0.05f });
	anim->Initialize(normalState);
	bIsPassable = false;
	bCollideWithOtherAttack = true;
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
