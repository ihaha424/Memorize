#include "TestPlayerController.h"
#include "../D2DGameEngine/InputComponent.h"
#include "../D2DGameEngine/CameraComponent.h"
#include "../D2DGameEngine/Pawn.h"
#include "../D2DGameEngine/Mouse.h"
#include "../D2DGameEngine/World.h"
#include "../Memorize/MovementComponent.h"

GPlayerController::GPlayerController(World* _world) : PlayerController(_world)
{
}

void GPlayerController::SetupInputComponent()
{
	inputComponent->ActionBinding(this, 0x00, &GPlayerController::MovePlayer, InputState::KeyDown, MouseInput);
	inputComponent->ActionBinding(this, DIK_Q, &GPlayerController::MovePlayer, InputState::KeyDown, KeyBoardInput);
}

void GPlayerController::BeginPlay()
{
	__super::BeginPlay();
	SetupInputComponent();
}

void GPlayerController::MovePlayer()
{
	Math::Vector2 mousePos = GetWorld()->ScreenToWorldPoint({ Mouse::curMousePosition.x, Mouse::curMousePosition.y });
	Math::Vector2 direction = mousePos - owner->GetLocation();
	direction.Normalize();
	owner->GetComponent<MovementComponent>()->SetDirection(direction);
	owner->GetComponent<MovementComponent>()->SetSpeed(500.0f);
}
