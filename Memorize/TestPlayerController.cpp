#include "TestPlayerController.h"
#include "../D2DGameEngine/InputComponent.h"
#include "../D2DGameEngine/CameraComponent.h"
#include "../D2DGameEngine/Pawn.h"
#include "../D2DGameEngine/Mouse.h"
#include "../D2DGameEngine/World.h"
#include "MovementComponent.h"

TestPlayerController::TestPlayerController(World* _world) : PlayerController(_world)
{
	SetTickProperties(TICK_UPDATE);

}

void TestPlayerController::SetupInputComponent()
{
	inputComponent->ActionBinding(this, 0x00, &TestPlayerController::MovePlayer, InputState::KeyDown, MouseInput);
}

void TestPlayerController::BeginPlay()
{
	__super::BeginPlay();
	SetupInputComponent();
}

void TestPlayerController::Update(float _dt)
{
	PlayerController::Update(_dt);
	if (Math::Vector2::Distance(destPos, owner->GetLocation()) < 2.f)
	{
		owner->GetComponent<MovementComponent>()->SetSpeed(0.f);
	}
}

void TestPlayerController::MovePlayer()
{
	destPos = GetWorld()->ScreenToWorldPoint({ Mouse::curMousePosition.x, Mouse::curMousePosition.y });
	Math::Vector2 direction = destPos - owner->GetLocation();
	direction.Normalize();
	owner->GetComponent<MovementComponent>()->SetDirection(direction);
	owner->GetComponent<MovementComponent>()->SetSpeed(500.0f);

	std::string tmp = {};
	tmp += std::to_string(destPos.x) + ", " + std::to_string(destPos.y);
	OBJ_INFO(tmp);
	//OBJ_INFO(msg);
}
