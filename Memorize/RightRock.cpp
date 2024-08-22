#include "RightRock.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"


RightRock::RightRock(World* _world) : Actor(_world)
{
	SetTickProperties(TICK_UPDATE | TICK_RENDER);
	AnimationBitmapComponent* abm = CreateComponent<AnimationBitmapComponent>();
	abm->SetSprite(L"TestResource/Main/03.png");
	abm->SliceSpriteSheet(677, 620, 0, 0, 0, 0);
	rootComponent = abm;
}

RightRock::~RightRock()
{
}

void RightRock::Update(float _dt)
{
	__super::Update(_dt);

	if (tweenRotate == nullptr)
	{
		tweenRotate = new DotTween<float>;
		tweenRotate->SetData(&rotateValue);
		tweenRotate->SetDuration(3);
		tweenRotate->SetStartPoint(-3);
		tweenRotate->SetEndPoint(3);
		tweenRotate->SetEasingEffect(EasingEffect::Linear);
		tweenRotate->SetStepAnimation(StepAnimation::StepLoopPingPong);
	}

	if (tweenUpAndDown == nullptr)
	{
		tweenUpAndDown = new DotTween<float>;
		tweenUpAndDown->SetData(&updownValue);
		tweenUpAndDown->SetDuration(3);
		tweenUpAndDown->SetStartPoint(-1);
		tweenUpAndDown->SetEndPoint(1);
		tweenUpAndDown->SetEasingEffect(EasingEffect::Linear);
		tweenUpAndDown->SetStepAnimation(StepAnimation::StepLoopPingPong);
	}

	tweenRotate->Update(_dt);
	tweenUpAndDown->Update(_dt);

	SetRotation(rotateValue);
	rootComponent->Translate(0, updownValue);
}
