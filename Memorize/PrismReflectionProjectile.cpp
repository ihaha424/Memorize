#include "PrismReflectionProjectile.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/Animator.h"

PrismReflectionProjectile::PrismReflectionProjectile(World* _world) : Projectile(_world)
{
	normalState->SetSprite(L"TestResource/Player/Skill/Skill_DarkSphere01.png");
	normalState->SliceSpriteSheet(140, 254, 0, 0, 0, 0);
	normalState->FrameResize(73);
	normalState->SetFrameDurations({ 0.05f });
	anim->Initialize(normalState);
	bIsPassable = true;
	bCollideWithOtherAttack = true;
}

PrismReflectionProjectile::~PrismReflectionProjectile()
{
}

void PrismReflectionProjectile::Update(float _dt)
{
	__super::Update(_dt);
}
