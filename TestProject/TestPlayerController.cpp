#include "TestPlayerController.h"
#include "../D2DGameEngine/InputComponent.h"
#include "../D2DGameEngine/CameraComponent.h"
#include "../D2DGameEngine/Pawn.h"
#include "../D2DGameEngine/Mouse.h"
#include "../D2DGameEngine/World.h"
#include "../Memorize/MovementComponent.h"

TestPlayerController::TestPlayerController(World* _world) : PlayerController(_world)
{
}

void TestPlayerController::SetupInputComponent()
{
	inputComponent->ActionBinding(this, 0x00, &TestPlayerController::MovePlayer, InputState::KeyDown, MouseInput);
	inputComponent->ActionBinding(this, DIK_Q, &TestPlayerController::MovePlayer, InputState::KeyDown, KeyBoardInput);
}

void TestPlayerController::BeginPlay()
{
	__super::BeginPlay();
	SetupInputComponent();
}

void TestPlayerController::MovePlayer()
{
	Math::Vector2 mousePos = GetWorld()->ScreenToWorldPoint({ Mouse::curMousePosition.x, Mouse::curMousePosition.y });
	Math::Vector2 direction = mousePos - owner->GetLocation();
	direction.Normalize();
	owner->GetComponent<MovementComponent>()->SetDirection(direction);
	owner->GetComponent<MovementComponent>()->SetSpeed(500.0f);
}
