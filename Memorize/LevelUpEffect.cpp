#include "LevelUpEffect.h"
#include "../D2DGameEngine/Animator.h"
#include "../D2DGameEngine/AnimationState.h"

LevelUpEffect::LevelUpEffect(World* _world)
	: AnimatorEffect(_world)
{
	verticalTween = new DotTween<float>();
	verticalTween->SetData(&vertical);
	verticalTween->SetDuration(1);
	verticalTween->SetEasingEffect(EasingEffect::OutQuad);
	verticalTween->SetStepAnimation(StepAnimation::StepOnceForward);
	verticalTween->SetStartPoint(0);
	verticalTween->SetEndPoint(-100);


	scaleTween = new DotTween<float>();
	scaleTween->SetData(&scale);
	scaleTween->SetDuration(1);
	scaleTween->SetEasingEffect(EasingEffect::InBack);
	scaleTween->SetStepAnimation(StepAnimation::StepOnceForward);
	scaleTween->SetStartPoint(0.5);
	scaleTween->SetEndPoint(2);
}

LevelUpEffect::~LevelUpEffect()
{
	delete verticalTween;
	delete scaleTween;
}

void LevelUpEffect::SetEffect(int index)
{
	AnimationState* state;


	state = animatorBitMap->CreateState<AnimationState>();
	switch (index)
	{
	case 0:
		state->SetSprite(L"TestResource/LevelUp/LEVELUP_Fire.png");
		break;
	case 1:
		state->SetSprite(L"TestResource/LevelUp/LEVELUP_Water.png");
		break;
	case 2:
		state->SetSprite(L"TestResource/LevelUp/LEVELUP_Light.png");
		break;
	case 3:
		state->SetSprite(L"TestResource/LevelUp/LEVELUP_Darkness.png");
		break;
	default:
		state->SetSprite(L"TestResource/LevelUp/LEVELUP_Fire.png");
		break;
	}
	state->SliceSpriteSheet(400, 200, 0, 0, 0, 0);
	state->FrameResize(24);
	state->SetFrameDurations({ 0.08f });
	state->Trigger(true);
	animatorBitMap->Initialize(state);
	SetAliveTime(2.f);
}

void LevelUpEffect::Update(float _dt)
{
	__super::Update(_dt);
	verticalTween->Update(_dt);
	scaleTween->Update(_dt);
	rootComponent->SetTranslation(pos.x, pos.y + vertical);
	rootComponent->SetScale(scale, scale);
}

void LevelUpEffect::BeginPlay()
{
	__super::BeginPlay();
	pos = GetLocation();
}
