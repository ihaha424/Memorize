#include "PrismReflectionProjectile.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/Animator.h"

PrismReflectionProjectile::PrismReflectionProjectile(World* _world) : Projectile(_world)
{
	normalState->SetSprite(L"TestResource/Player/Skill/Skill_PrismReflection01.png");
	normalState->SliceSpriteSheet(240, 181, 0, 0, 0, 0);
	normalState->FrameResize(120);
	normalState->SetFrameDurations({ 2.f/120 });
	anim->Initialize(normalState);
	bIsPassable = true;
	bCollideWithOtherAttack = true;

	bIsPassable = false;
	bCollideWithOtherAttack = true;
	bHasEnding = false;
}

PrismReflectionProjectile::~PrismReflectionProjectile()
{
}

void PrismReflectionProjectile::Update(float _dt)
{
	__super::Update(_dt);
}
