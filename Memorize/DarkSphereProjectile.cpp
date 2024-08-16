#include "DarkSphereProjectile.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/Animator.h"
#include "MovementComponent.h"

DarkSphereProjectile::DarkSphereProjectile(World* _world) : Projectile(_world)
{
	animState->SetSprite(L"TestResource/Player/Skill/Skill_DarkSphere01.png");
	animState->SliceSpriteSheet(140, 254, 0, 0, 0, 0);
	animState->FrameResize(73);
	animState->SetFrameDurations({ 0.025f });
	anim->Initialize(animState);
}

DarkSphereProjectile::~DarkSphereProjectile()
{
}