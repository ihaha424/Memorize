#include "Title.h"
#include "D2DGameEngine/BitmapComponent.h"


Title::Title(World* _world) : Actor(_world)
{
	SetTickProperties(TICK_UPDATE | TICK_RENDER);
	BitmapComponent* abm = CreateComponent<BitmapComponent>();
	abm->SetSprite(L"TestResource/Main/Title.png");
	rootComponent = abm;
}

Title::~Title()
{
}

void Title::Update(float _dt)
{
	__super::Update(_dt);

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

	tweenUpAndDown->Update(_dt);

	rootComponent->Translate(0, updownValue);
}
