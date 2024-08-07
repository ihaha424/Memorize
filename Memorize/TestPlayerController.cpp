#include "TestPlayerController.h"
#include "../D2DGameEngine/InputComponent.h"
#include "../D2DGameEngine/CameraComponent.h"
#include "../D2DGameEngine/Pawn.h"
#include "../D2DGameEngine/Mouse.h"
#include "../D2DGameEngine/World.h"
#include "MovementComponent.h"
#include "Fireball.h"
#include "ChasingWaterBall.h"

TestPlayerController::TestPlayerController(World* _world) : PlayerController(_world)
{
	SetTickProperties(TICK_UPDATE);

}

void TestPlayerController::SetupInputComponent()
{
	inputComponent->ActionBinding(this, 0x00, &TestPlayerController::MovePlayer, InputState::KeyDown, MouseInput);
	inputComponent->ActionBinding(this, DIK_Q, &TestPlayerController::StartSkill<Fireball>, InputState::KeyDown, KeyBoardInput);
	inputComponent->ActionBinding(this, DIK_W, &TestPlayerController::StartSkill<ChasingWaterBall>, InputState::KeyDown, KeyBoardInput);
}

void TestPlayerController::EndSkill()
{
	nowSkill = nullptr;
}

void TestPlayerController::BeginPlay()
{
	//각 스킬의 인스턴스를 미리 생성
	skills = {
		{ std::type_index(typeid(Fireball)), CreateComponent<Fireball>()},
		{ std::type_index(typeid(ChasingWaterBall)), CreateComponent<ChasingWaterBall>()},
	};

	//스킬의 오너 설정
	for (auto skill : skills)
	{
		skill.second->SetPlayer(this);
	}


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
