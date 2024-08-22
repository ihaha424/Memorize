#include "Rock01.h"
#include "D2DGameEngine/BitmapComponent.h"


Rock01::Rock01(World* _world) : Actor(_world)
{
	SetTickProperties(TICK_UPDATE | TICK_RENDER);
	BitmapComponent* abm = CreateComponent<BitmapComponent>();
	abm->SetSprite(L"TestResource/Main/Title_rocks01.png");
	rootComponent = abm;
}

Rock01::~Rock01()
{
}

void Rock01::Update(float _dt)
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
		tweenUpAndDown->SetStartPoint(-0.1);
		tweenUpAndDown->SetEndPoint(0.1);
		tweenUpAndDown->SetEasingEffect(EasingEffect::Linear);
		tweenUpAndDown->SetStepAnimation(StepAnimation::StepLoopPingPong);
	}

	tweenRotate->Update(_dt);
	tweenUpAndDown->Update(_dt);

	SetRotation(rotateValue);
	rootComponent->Translate(0, updownValue);
}
