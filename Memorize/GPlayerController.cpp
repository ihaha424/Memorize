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

	InitializeSkill();

	playerFSMComponent = CreateComponent<PlayerFSMComponent>();

}

void GPlayerController::SetupInputComponent()
{
	inputComponent->ActionBinding(this, DIK_Q, &GPlayerController::Fire, InputState::KeyDown, KeyBoardInput);
	inputComponent->ActionBinding(this, DIK_W, &GPlayerController::Water, InputState::KeyDown, KeyBoardInput);
	inputComponent->ActionBinding(this, DIK_E, &GPlayerController::Light, InputState::KeyDown, KeyBoardInput);
	inputComponent->ActionBinding(this, DIK_R, &GPlayerController::Dark, InputState::KeyDown, KeyBoardInput);
	inputComponent->ActionBinding(this, DIK_TAB, &GPlayerController::Memorize, InputState::KeyDown, KeyBoardInput);
	
	inputComponent->ActionBinding(this, 0, &GPlayerController::Attack, InputState::KeyDown, MouseInput);
	inputComponent->ActionBinding(this, 1, &GPlayerController::Move, InputState::KeyDown, MouseInput);
}

void GPlayerController::InitializeSkill()
{
	//�� ��ų�� �ν��Ͻ��� �̸� ����
	/*skills = {
		{ std::type_index(typeid(Fireball)), CreateComponent<Fireball>()},
		{ std::type_index(typeid(ChasingWaterBall)), CreateComponent<ChasingWaterBall>()},
		{ std::type_index(typeid(Meteor)), CreateComponent<Meteor>()},
	};*/
	CreateSkill<Fireball>();
	CreateSkill<ChasingWaterBall>();
	CreateSkill<Meteor>();
}

void GPlayerController::EndSkill()
{
	nowSkill = nullptr;
}

void GPlayerController::BeginPlay()
{
	__super::BeginPlay();
	
	//��Ʈ�ѷ��� ��Ʈ ������Ʈ�� �÷��̾� ��Ʈ�� �ڽ����� ����
	GetPlayer()->rootComponent->AddChild(rootComponent);

	//��ų�� ���� ����
	for (auto skill : skills)
	{
		skill.second->SetPlayer(this);
	}

	SetupInputComponent();


	//Skill Initialize
	InitializeSkillInfo();
	InitializeMemorize();
}

void GPlayerController::Update(float _dt)
{
	PlayerController::Update(_dt);
	if (Math::Vector2::Distance(destPos, owner->GetLocation()) < 2.f)
	{
		owner->GetComponent<MovementComponent>()->SetSpeed(0.f);
	}
}

bool GPlayerController::AddSkillInfo(int index)
{
	if (curSkillInfo.element == ESkillElement::SE_END)
	{
		curSkillInfo.element = (ESkillElement)index;
		return false;
	}
	else if (curSkillInfo.type == ESkillType::ST_END)
	{
		curSkillInfo.type = (ESkillType)index;
		return true;
	}
	return true;
}
bool GPlayerController::CheckSkillInfo()
{
	if (curSkillInfo.element != ESkillElement::SE_END &&
		curSkillInfo.type != ESkillType::ST_END)
		return true;
	return false;
}

void GPlayerController::InitializeSkillInfo()
{
	curSkillInfo.element = ESkillElement::SE_END;
	curSkillInfo.type = ESkillType::ST_END;
}

bool GPlayerController::InputMemorize()
{
	if (MemorizeSkill.element != ESkillElement::SE_END &&
		MemorizeSkill.type != ESkillType::ST_END)
		return true;
	return false;
}

#include "D2DGameEngine/Debug.h"
#include "GCameraComponent.h"
void GPlayerController::Fire()		{ playerFSMComponent->InputKey(InputEvent::Fire); }
void GPlayerController::Water() { playerFSMComponent->InputKey(InputEvent::Water); }
void GPlayerController::Light() { playerFSMComponent->InputKey(InputEvent::Light); }
void GPlayerController::Dark() { playerFSMComponent->InputKey(InputEvent::Dark); }
void GPlayerController::Attack() { 
	//Math::Vector2 destPos = GetWorld()->ScreenToWorldPoint({ Mouse::curMousePosition.x, Mouse::curMousePosition.y });
	//LOG_WARNING(dbg::text(GetPlayer()->GetLocation().x, ", ", GetPlayer()->GetLocation().y));
	//LOG_INFO(dbg::text(destPos.x, ", ", destPos.y));

	//destPos = GetPlayer()->GetComponent<GCameraComponent>()->GetComponentLocation();
	//LOG_MESSAGE(dbg::text(destPos.x, ", ", destPos.y));
	GetPlayer()->GetComponent<MovementComponent>()->SetSpeed(0.f);


	//playerFSMComponent->InputKey(InputEvent::Attack); 
}
void GPlayerController::Move() { playerFSMComponent->InputKey(InputEvent::Move); }

void GPlayerController::Memorize() {playerFSMComponent->InputKey(InputEvent::Memorize);}
