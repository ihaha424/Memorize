#include "PrismReflectionProjectile.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/Animator.h"

PrismReflectionProjectile::PrismReflectionProjectile(World* _world) : Projectile(_world)
{
	animState->SetSprite(L"TestResource/Player/Skill/Skill_DarkSphere01.png");
	animState->SliceSpriteSheet(170, 254, 0, 0, 0, 0);
	animState->FrameResize(73);
	animState->SetFrameDurations({ 0.05f });
	anim->Initialize(animState);
}

PrismReflectionProjectile::~PrismReflectionProjectile()
{
}

void PrismReflectionProjectile::Update(float _dt)
{
	__super::Update(_dt);
}
