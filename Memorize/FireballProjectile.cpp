#include "FireballProjectile.h"
#include "../D2DGameEngine/AnimationState.h"
#include "../D2DGameEngine/Animator.h"
#include "../D2DGameEngine/BoxComponent.h"
#include "MovementComponent.h"

FireballProjectile::FireballProjectile(World* _world) : Projectile(_world)
{
	normalState->SetSprite(L"TestResource/Player/Skill/Skill_DarkSphere01.png");
	normalState->SliceSpriteSheet(140, 254, 0, 0, 0, 0);
	normalState->FrameResize(73);
	normalState->SetFrameDurations({ 0.05f });
	anim->Initialize(normalState);

	endingState->SetSprite(L"TestResource/Player/Skill/Skill_DarkSphere02.png");
	endingState->SliceSpriteSheet(150, 150, 0, 0, 0, 0);
	endingState->FrameResize(7);
	endingState->SetFrameDurations({ 0.14285f });

	box->InitBoxExtent({ 140, 254 });

	endingTime = 1.f;
	bIsPassable = false;
	bCollideWithOtherAttack = true;
	bHasEnding = true;
}

FireballProjectile::~FireballProjectile()
{
}
