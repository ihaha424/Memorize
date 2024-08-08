#include "GPlayerController.h"
#include "../D2DGameEngine/InputComponent.h"
#include "../D2DGameEngine/CameraComponent.h"
#include "../D2DGameEngine/Pawn.h"
#include "../D2DGameEngine/Mouse.h"
#include "../D2DGameEngine/World.h"
#include "PlayerFSMComponent.h"
#include "MovementComponent.h"
#include "Fireball.h"
#include "ChasingWaterBall.h"
#include "Meteor.h"
#include "Player.h"

GPlayerController::GPlayerController(World* _world) : PlayerController(_world)
{
	SetTickProperties(TICK_UPDATE);

	rootComponent = CreateComponent<SceneComponent>();

	//각 스킬의 인스턴스를 미리 생성
	skills = {
		{ std::type_index(typeid(Fireball)), CreateComponent<Fireball>()},
		{ std::type_index(typeid(ChasingWaterBall)), CreateComponent<ChasingWaterBall>()},
		{ std::type_index(typeid(Meteor)), CreateComponent<Meteor>()},
	};

	playerFSMComponent = CreateComponent<PlayerFSMComponent>();

}

void GPlayerController::SetupInputComponent()
{
	inputComponent->ActionBinding(this, DIK_Q, &GPlayerController::Fire, InputState::KeyDown, KeyBoardInput);
	inputComponent->ActionBinding(this, DIK_W, &GPlayerController::Water, InputState::KeyDown, KeyBoardInput);
	inputComponent->ActionBinding(this, DIK_E, &GPlayerController::Light, InputState::KeyDown, KeyBoardInput);
	inputComponent->ActionBinding(this, DIK_R, &GPlayerController::Dark, InputState::KeyDown, KeyBoardInput);
	inputComponent->ActionBinding(this, 0, &GPlayerController::Attack, InputState::KeyDown, MouseInput);
	inputComponent->ActionBinding(this, 1, &GPlayerController::Move, InputState::KeyDown, MouseInput);
}

void GPlayerController::EndSkill()
{
	nowSkill = nullptr;
}

void GPlayerController::BeginPlay()
{
	__super::BeginPlay();
	
	//컨트롤러의 루트 컴포넌트를 플레이어 루트의 자식으로 설정
	GetPlayer()->rootComponent->AddChild(rootComponent);

	//스킬의 오너 설정
	for (auto skill : skills)
	{
		skill.second->SetPlayer(this);
	}

	SetupInputComponent();
}

void GPlayerController::Update(float _dt)
{
	PlayerController::Update(_dt);
	if (Math::Vector2::Distance(destPos, owner->GetLocation()) < 2.f)
	{
		owner->GetComponent<MovementComponent>()->SetSpeed(0.f);
	}
}

void GPlayerController::Fire()		{ playerFSMComponent->InputKey(PlayerFSMComponent::InputEvent::Fire); }
void GPlayerController::Water() { playerFSMComponent->InputKey(PlayerFSMComponent::InputEvent::Water); }
void GPlayerController::Light() { playerFSMComponent->InputKey(PlayerFSMComponent::InputEvent::Light); }
void GPlayerController::Dark() { playerFSMComponent->InputKey(PlayerFSMComponent::InputEvent::Dark); }
void GPlayerController::Attack() { playerFSMComponent->InputKey(PlayerFSMComponent::InputEvent::Attack); }
void GPlayerController::Move() { playerFSMComponent->InputKey(PlayerFSMComponent::InputEvent::Move); }