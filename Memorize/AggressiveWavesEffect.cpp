#include "AggressiveWavesEffect.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/Animator.h"

AggressiveWavesEffect::AggressiveWavesEffect(World* _world) : Projectile(_world)
{
	animState->SetSprite(L"TestResource/Player/Skill/Skill_DarkSphere01.png");
	animState->SliceSpriteSheet(170, 254, 0, 0, 0, 0);
	animState->FrameResize(73);
	animState->SetFrameDurations({ 0.05f });
	anim->Initialize(animState);
	Inactivate();
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
