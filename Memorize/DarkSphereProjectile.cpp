#include "DarkSphereProjectile.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/BoxComponent.h"
#include "MovementComponent.h"

DarkSphereProjectile::DarkSphereProjectile(World* _world) : Projectile(_world)
{
	normalState->SetSprite(L"TestResource/Player/Skill/Skill_DarkSphere01.png");
	normalState->SliceSpriteSheet(140, 254, 0, 0, 0, 0);
	normalState->FrameResize(73);
	normalState->SetFrameDurations({ 0.025f });
	anim->Initialize(normalState);

	endingState->SetSprite(L"TestResource/Player/Skill/Skill_DarkSphere02.png");
	endingState->SliceSpriteSheet(150, 150, 0, 0, 0, 0);
	endingState->FrameResize(7);
	endingState->SetFrameDurations({ 0.333333f });

	endingTime = 2.f;
	duration = 1.f;


	box->InitBoxExtent({ 140, 254});
}

DarkSphereProjectile::~DarkSphereProjectile()
{
}

void DarkSphereProjectile::Update(float _dt)
{
	__super::Update(_dt);


}
