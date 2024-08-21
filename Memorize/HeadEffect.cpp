#include "HeadEffect.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/Animator.h"

HeadEffect::HeadEffect(World* _world) : AnimatorEffect(_world)
{
	
}

void HeadEffect::SetEffect(int index)
{
	AnimationState* state = animatorBitMap->CreateState<AnimationState>();
	if (index == 0)
	{
		state->SetSprite(L"TestResource/Player/Skill/Skill_Memorize.png");
		state->SliceSpriteSheet(512, 512, 0, 0, 0, 0);
		state->SetFrameDurations({ 1.0f / 44 });
		SetAliveTime(1.0f);
	}
	
	else if (index == 1)
	{
		state->SetSprite(L"TestResource/Player/Skill/Skill_Heal01.png");
		state->SliceSpriteSheet(500, 500, 0, 0, 0, 0);
		state->SetFrameDurations({ 2.0f / 24 });
		SetAliveTime(1.0f);
	}
	else if (index == 2)
	{
		state->SetSprite(L"TestResource/Player/Skill/Skill_ElementalExplosion.png");
		state->SliceSpriteSheet(450, 512, 0, 0, 0, 0);
		state->SetFrameDurations({ 2.0f / 72 });
		SetAliveTime(2.0f);
	}
	else if (index == 3)
	{
		state->SetSprite(L"TestResource/Player/Skill/Skill_Purification.png");
		state->SliceSpriteSheet(1000, 1000, 0, 0, 0, 0);
		state->SetFrameDurations({ 2.0f / 60 });
		SetAliveTime(2.0f);
	}
	animatorBitMap->Initialize(state);
}

void HeadEffect::SetPosition(Math::Vector2* _pos)
{
	playerPos = _pos;
}

void HeadEffect::FixedUpdate(float _fixedRate)
{
	__super::FixedUpdate(_fixedRate);

	SetLocation(playerPos->x, playerPos->y - 100);
}
