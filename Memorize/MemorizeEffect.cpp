#include "MemorizeEffect.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/Animator.h"

MemorizeEffect::MemorizeEffect(World* _world) : AnimatorEffect(_world)
{
	
}

void MemorizeEffect::StartEffect()
{
	
	AnimationState* state = animatorBitMap->CreateState<AnimationState>();
	state->SetSprite(L"TestResource/Player/Skill/Skill_Memorize.png");
	state->SliceSpriteSheet(512, 512, 0, 0, 0, 0);
	state->SetFrameDurations({ 1.0f / 44 });

	animatorBitMap->Initialize(state);
	SetAliveTime(1.0f);

}

void MemorizeEffect::SetPosition(Math::Vector2* _pos)
{
	playerPos = _pos;
}

void MemorizeEffect::FixedUpdate(float _fixedRate)
{
	__super::FixedUpdate(_fixedRate);

	SetLocation(playerPos->x, playerPos->y);
}
