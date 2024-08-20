#include "BossHitEffect.h"
#include "../D2DGameEngine/Animator.h"
#include "../D2DGameEngine/AnimationState.h"

BossHitEffect::BossHitEffect(World* _world)
	: AnimatorEffect(_world)
{

}

void BossHitEffect::SetEffect(int index)
{
	AnimationState* state;


	state = animatorBitMap->CreateState<AnimationState>();
	switch (index)
	{
	case 0:
		state->SetSprite(L"TestResource/Player/Effect/Boss_Hit_Fire.png");
		break;
	case 1:
		state->SetSprite(L"TestResource/Player/Effect/Boss_Hit_Water.png");
	break;
	case 2:
		state->SetSprite(L"TestResource/Player/Effect/Boss_Hit_Light.png");
	break;
	case 3:
		state->SetSprite(L"TestResource/Player/Effect/Boss_Hit_Dark.png");
	break;
	default:
		state->SetSprite(L"TestResource/Player/Effect/Boss_Hit_Fire.png");
		break;
	}
	state->SliceSpriteSheet(400, 400, 0, 0, 0, 0);
	state->FrameResize(30);
	state->SetFrameDurations({ 0.034f });
	state->Trigger(true);
	animatorBitMap->Initialize(state);
	SetAliveTime(1.f);
}
