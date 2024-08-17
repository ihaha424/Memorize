#include "AggressiveWavesEffect.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/Animator.h"

AggressiveWavesEffect::AggressiveWavesEffect(World* _world) : Projectile(_world)
{
	normalState->SetSprite(L"TestResource/Player/Skill/Skill_DarkSphere01.png");
	normalState->SliceSpriteSheet(140, 254, 0, 0, 0, 0);
	normalState->FrameResize(73);
	normalState->SetFrameDurations({ 0.05f });
	anim->Initialize(normalState);
	Inactivate();
	bIsPassable = true;
	bCollideWithOtherAttack = true;
}

AggressiveWavesEffect::~AggressiveWavesEffect()
{
}

void AggressiveWavesEffect::BeginPlay()
{
	__super::BeginPlay();
}

void AggressiveWavesEffect::Update(float _dt)
{
	__super::Update(_dt);
}
