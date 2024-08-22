#include "FadeInFadeOut.h"
#include "D2DGameEngine/UIImage.h"

FadeInFadeOut::FadeInFadeOut(World* _world)
	: UIPanel(_world)
{
	SetPosition(1920 / 2, 1080 / 2);
	SetZOrder(100);
	image = CreateUI<UIImage>(L"FadeIn");
	image->SetPosition(0, 0);

	alphaTween = new DotTween<float>();
	alphaTween->SetData(&alpha);
	alphaTween->SetDuration(2);
	alphaTween->SetEasingEffect(EasingEffect::Linear);
	alphaTween->SetStepAnimation(StepAnimation::StepOnceForward);
}

FadeInFadeOut::~FadeInFadeOut()
{
	delete alphaTween;
}

void FadeInFadeOut::BeginPlay()
{
	__super::BeginPlay();

	if (color)
		image->SetSprite(L"TestResource/FadeInFadeOut/Full_Black.png");
	else
		image->SetSprite(L"TestResource/FadeInFadeOut/Full_White.png");

	if (inOut)
	{
		alphaTween->SetStartPoint(1);
		alphaTween->SetEndPoint(0);
	}
	else
	{
		alphaTween->SetStartPoint(0);
		alphaTween->SetEndPoint(1);
	}
}

void FadeInFadeOut::Update(float _dt)
{
	__super::Update(_dt);
	alphaTween->Update(_dt);
	image->SetOpacity(alpha);
	if (alphaTween->GetIsFinish())
	{
		if (nullptr != func)
			func();
	}
}
