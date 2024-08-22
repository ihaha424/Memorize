#include "LeftRock.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"

LeftRock::LeftRock(World* _world) : Actor(_world)
{
	SetTickProperties(TICK_UPDATE | TICK_RENDER);
	renderLayer = 0;

	AnimationBitmapComponent* abm = CreateComponent<AnimationBitmapComponent>();
	abm->SetSprite(L"TestResource/Main/04.png");
	abm->SliceSpriteSheet(322, 473, 0, 0, 0, 0);
	rootComponent = abm;
}

LeftRock::~LeftRock()
{
}

void LeftRock::Update(float _dt)
{
	__super::Update(_dt);


	if (tweenRotate == nullptr)
	{
		tweenRotate = new DotTween<float>;
		tweenRotate->SetData(&rotateValue);
		tweenRotate->SetDuration(2);
		tweenRotate->SetStartPoint(-2);
		tweenRotate->SetEndPoint(2);
		tweenRotate->SetEasingEffect(EasingEffect::Linear);
		tweenRotate->SetStepAnimation(StepAnimation::StepLoopPingPong);
	}

	if (tweenUpAndDown == nullptr)
	{
		tweenUpAndDown = new DotTween<float>;
		tweenUpAndDown->SetData(&updownValue);
		tweenUpAndDown->SetDuration(3);
		tweenUpAndDown->SetStartPoint(0.3);
		tweenUpAndDown->SetEndPoint(-0.3);
		tweenUpAndDown->SetEasingEffect(EasingEffect::Linear);
		tweenUpAndDown->SetStepAnimation(StepAnimation::StepLoopPingPong);
	}

	tweenRotate->Update(_dt);
	tweenUpAndDown->Update(_dt);

	SetRotation(rotateValue);
	rootComponent->Translate(0, updownValue);
}
