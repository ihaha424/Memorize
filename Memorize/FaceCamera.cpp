#include "FaceCamera.h"
#include "../D2DGameEngine/Actor.h"
#include "../D2DGameEngine/Canvas.h"
#include "FadeInFadeOut.h"
#include "../D2DGameEngine/World.h"

FaceCamera::FaceCamera(Actor* _owner)
	: CameraComponent(_owner)
{
	SetTickProperties(TICK_UPDATE | TICK_POST_UPDATE | TICK_RENDER);
	isVisible = true;
}

FaceCamera::~FaceCamera()
{
}

void FaceCamera::BeginPlay()
{
	__super::BeginPlay();
	cameraZoomDotTween = new DotTween<float>();
	cameraZoomDotTween->SetData(&cameraZoom);
	cameraZoomDotTween->SetDuration(3);
	cameraZoomDotTween->SetEasingEffect(EasingEffect::InSine);
	cameraZoomDotTween->SetStepAnimation(StepAnimation::StepOncePingPong);
	cameraZoomDotTween->SetStartPoint(0.5f);
	cameraZoomDotTween->SetEndPoint(2);

	Math::Vector2 playerPos = Player->GetLocation();
	Math::Vector2 bossPos = Boss->GetLocation();
	cameraXTween = new DotTween<float>();
	cameraXTween->SetData(&cameraX);
	cameraXTween->SetDuration(3.f);
	cameraXTween->SetEasingEffect(EasingEffect::InSine);
	cameraXTween->SetStepAnimation(StepAnimation::StepOnceForward);
	cameraXTween->SetStartPoint(playerPos.x);
	cameraXTween->SetEndPoint(bossPos.x);
}

void FaceCamera::Update(float _dt)
{
	__super::Update(_dt);
	cameraXTween->Update(_dt);
	cameraZoomDotTween->Update(_dt);
	SetTranslation(cameraX, 0);
	SetScale(cameraZoom, cameraZoom);
	if (cameraXTween->GetIsFinish())
		delayTime -= _dt;
	if (delayTime < 0.f)
	{
		Math::Vector2 playerPos = Player->GetLocation();
		Math::Vector2 bossPos = Boss->GetLocation();
		cameraXTween->SetStartPoint(bossPos.x);
		cameraXTween->SetEndPoint(playerPos.x);
		cameraXTween->ResetTime();
		cameraZoomDotTween->ResetTime();
		FadeInFadeOut* playerDie = GetWorld()->GetCanvas()->CreatePannel<FadeInFadeOut>(L"TestLevelFadeOut");
		playerDie->SetFaidInOut(false);
		playerDie->SetFinishFunction([this]()->void {
			GetWorld()->SetNextScene(L"TestLevel");
			});
		playerDie->alphaTween->SetDuration(1.5f);
		playerDie->alphaTween->SetEasingEffect(EasingEffect::InQuart);
		delayTime = 10.f;
	}
}

void FaceCamera::SetActor(Actor* _Player, Actor* _Boss)
{
	Player = _Player;
	Boss = _Boss;
}

