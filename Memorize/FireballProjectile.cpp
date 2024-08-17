#include "FireballProjectile.h"
#include "../D2DGameEngine/AnimationState.h"
#include "../D2DGameEngine/Animator.h"
#include "../D2DGameEngine/BoxComponent.h"
#include "MovementComponent.h"

FireballProjectile::FireballProjectile(World* _world) : Projectile(_world)
{
	normalState->SetSprite(L"TestResource/Player/Skill/Skill_Fireball01.png");
	normalState->SliceSpriteSheet(137, 254, 0, 0, 0, 0);
	normalState->FrameResize(48);
	normalState->SetFrameDurations({ 2.f/48 });
	anim->Initialize(normalState);

	endingState->SetSprite(L"TestResource/Player/Skill/Skill_Fireball02.png");
	endingState->SliceSpriteSheet(512,512, 0, 0, 0, 0);
	endingState->FrameResize(48);
	endingState->SetFrameDurations({ 0.14285f });

	box->InitBoxExtent({ 137, 254 });

	endingTime = 1.f;
	bIsPassable = false;
	bCollideWithOtherAttack = true;
	bHasEnding = true;
}

FireballProjectile::~FireballProjectile()
{
}
