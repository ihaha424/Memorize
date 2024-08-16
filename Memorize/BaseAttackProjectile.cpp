#include "BaseAttackProjectile.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/Animator.h"

BaseAttackProjectile::BaseAttackProjectile(World* _world) : Projectile(_world)
{
	animState->SetSprite(L"TestResource/Player/Skill/Skill_DarkSphere01.png");
	animState->SliceSpriteSheet(170, 254, 0, 0, 0, 0);
	animState->FrameResize(73);
	animState->SetFrameDurations({ 0.05f });
	anim->Initialize(animState);
	Inactivate();
}

BaseAttackProjectile::~BaseAttackProjectile()
{
}

void BaseAttackProjectile::OnBeginOverlap(Actor* other)
{
	__super::OnBeginOverlap(other);


}
