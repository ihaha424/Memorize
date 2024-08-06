#include "TestPlayerController.h"
#include "../D2DGameEngine/InputComponent.h"
#include "../D2DGameEngine/Pawn.h"

void TestPlayerController::SetupInputComponent()
{
	inputComponent->ActionBinding(this, 0x00, &TestPlayerController::MovePlayer, InputState::KeyDown, MouseInput);
}

void TestPlayerController::BeginPlay()
{
	__super::BeginPlay();
	SetupInputComponent();
}

void TestPlayerController::MovePlayer()
{
	Math::Vector2 nowPos = owner->GetLocation();
	owner->SetLocation(nowPos.x + 100, nowPos.y);
}
