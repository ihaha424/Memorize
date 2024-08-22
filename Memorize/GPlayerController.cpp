#include "GPlayerController.h"
#include "../D2DGameEngine/InputComponent.h"
#include "../D2DGameEngine/CameraComponent.h"
#include "../D2DGameEngine/Pawn.h"
#include "../D2DGameEngine/Mouse.h"
#include "../D2DGameEngine/World.h"
#include "../D2DGameEngine/Canvas.h"
#include "../D2DGameEngine/RandomGenerator.h"
#include "PlayerFSMComponent.h"
#include "MovementComponent.h"
#include "SkillList.h"
#include "Player.h"
#include "ElementsPanel.h"
#include "BossSkillActor.h"
#include "ElementalMasterComponent.h"

GPlayerController::GPlayerController(World* _world) : PlayerController(_world)
{
	SetTickProperties(TICK_UPDATE);

	rootComponent = CreateComponent<SceneComponent>();

	InitializeSkill();

	playerFSMComponent = CreateComponent<PlayerFSMComponent>();

	AddEventHandler(&GPlayerController::DisfellEvent);

	OnBeginDisfell = new Signal<int, int>;
	OnDoingDisfell = new Signal<int, int>;
	OnFlash = new Signal<>;
	OnMemorize = new Signal<>;
	OffFlash = new Signal<>;
	OffMemorize = new Signal<>;
	
	elementalMasterComponent = CreateComponent<ElementalMasterComponent>();
	elementalMasterComponent->SetStatus(OS_INACTIVE);
}

GPlayerController::~GPlayerController()
{
	inputComponent->DeleteCommand(this, DIK_Q, InputState::KeyDown, KeyBoardInput);
	inputComponent->DeleteCommand(this, DIK_W, InputState::KeyDown, KeyBoardInput);
	inputComponent->DeleteCommand(this, DIK_E, InputState::KeyDown, KeyBoardInput);
	inputComponent->DeleteCommand(this, DIK_R, InputState::KeyDown, KeyBoardInput);
	inputComponent->DeleteCommand(this, DIK_TAB, InputState::KeyDown, KeyBoardInput);
	inputComponent->DeleteCommand(this, DIK_SPACE, InputState::KeyDown, KeyBoardInput);
	inputComponent->DeleteCommand(this, DIK_ESCAPE, InputState::KeyDown, KeyBoardInput);

	inputComponent->DeleteCommand(this, { DIK_A, DIK_S, DIK_D }, InputState::KeyDown, KeyBoardInput);

	inputComponent->DeleteCommand(this, 0, InputState::KeyDown, MouseInput);
	inputComponent->DeleteCommand(this, 1, InputState::KeyDown, MouseInput);

	delete OnBeginDisfell;
	delete OnDoingDisfell;
	delete OnFlash;
	delete OnMemorize;
	delete OffFlash;
	delete OffMemorize;
}

void GPlayerController::SetupInputComponent()
{
	inputComponent->ActionBinding(this, DIK_Q, &GPlayerController::Fire, InputState::KeyDown, KeyBoardInput);
	inputComponent->ActionBinding(this, DIK_W, &GPlayerController::Water, InputState::KeyDown, KeyBoardInput);
	inputComponent->ActionBinding(this, DIK_E, &GPlayerController::Light, InputState::KeyDown, KeyBoardInput);
	inputComponent->ActionBinding(this, DIK_R, &GPlayerController::Dark, InputState::KeyDown, KeyBoardInput);
	inputComponent->ActionBinding(this, DIK_TAB, &GPlayerController::Memorize, InputState::KeyDown, KeyBoardInput);
	inputComponent->ActionBinding(this, DIK_SPACE, &GPlayerController::Teleport, InputState::KeyDown, KeyBoardInput);
	inputComponent->ActionBinding(this, DIK_ESCAPE, &GPlayerController::Cancellation, InputState::KeyDown, KeyBoardInput);

	inputComponent->ActionBinding(this, { DIK_A, DIK_S, DIK_D}, &GPlayerController::Cheat, InputState::KeyDown, KeyBoardInput);
	
	inputComponent->ActionBinding(this, 0, &GPlayerController::Attack, InputState::KeyDown, MouseInput);
	inputComponent->ActionBinding(this, 1, &GPlayerController::Move, InputState::KeyDown, MouseInput);

}

void GPlayerController::InitializeSkill()
{
	//각 스킬의 인스턴스를 미리 생성
	/*skills = {
		{ std::type_index(typeid(Fireball)), CreateComponent<Fireball>()},
		{ std::type_index(typeid(ChasingWaterBall)), CreateComponent<ChasingWaterBall>()},
		{ std::type_index(typeid(Meteor)), CreateComponent<Meteor>()},
	};*/
	CreateSkill<Fireball>();
	CreateSkill<Meteor>();
	CreateSkill<Enchant>();
	CreateSkill<ElementalExplosion>();

	CreateSkill<ChasingWaterBall>();
	CreateSkill<AggressiveWaves>();
	CreateSkill<ManaOverload>();
	CreateSkill<Heal>();

	CreateSkill<PrismReflection>();
	CreateSkill<LightStream>();
	CreateSkill<MPMaxIncrease>();
	CreateSkill<Purification>();
	
	CreateSkill<DarkSphere>();
	CreateSkill<TighteningCircle>();
	CreateSkill<MPRecovery>();
	CreateSkill<ElementalMaster>();

	CreateSkill<BasicAttack>();
}

int GPlayerController::GetPlayerCastingIndex()
{
	if (playerFSMComponent->GetCurState() == L"PlayerAttackReady")
		return FindCurSkiil()->GetCommandSize();
	if (playerFSMComponent->GetCurState() != L"PlayerCasting")
		return -1;
	PlayerCasting* castingState = static_cast<PlayerCasting*>(playerFSMComponent->GetCurStateClass());
	return castingState->index;
}

bool GPlayerController::isPlayerAfterCasting()
{
	return playerFSMComponent->GetCurState() == L"PlayerAttackReady" 
		|| playerFSMComponent->GetCurState() == L"PlayerAttack"
		|| playerFSMComponent->GetCurState() == L"PlayerDisfell";
}

std::wstring GPlayerController::GetPlayerState()
{
	return playerFSMComponent->GetCurState();
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

	SetupInputComponent();

	for (auto& skill : skills)
	{
		skill.second->SetPlayer(this);
		skill.second->Initialize();
	}

	//Skill Initialize
	InitializeSkillInfo();
	InitializeMemorize();
}

void GPlayerController::Update(float _dt)
{
	PlayerController::Update(_dt);

	playerFSMComponent->OneTImeOneInput = false;

	if (Math::Vector2::Distance(destPos, player->GetLocation()) < 12.f)
	{
		player->GetComponent<MovementComponent>()->SetSpeed(0.f);
	}

	// ++RigidBody에 속도의 방향에 대한 정보로 x filp하기

	if (playerFSMComponent->GetCurState() != L"PlayerDisfell" && playerFSMComponent->GetCurState() != L"PlayerBlinking")
	{
		OnDoingDisfell->Emit(-1, 0);
	}

	if (playerFSMComponent->GetCurState() != L"PlayerBlinking")
		OffFlash->Emit();
}

void GPlayerController::SetRandomSkillReady()
{
	curSkillInfo.element = (ESkillElement)Random::Get<int>(3);
	curSkillInfo.type = ST_PROJECTILE;

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

bool GPlayerController::CheckMemorize()
{
	if (MemorizeSkill.element != ESkillElement::SE_END &&
		MemorizeSkill.type != ESkillType::ST_END)
		return true;
	return false;
}

void GPlayerController::OnManaDepleted()
{
	GetWorld()->GetCanvas()->ShowPanel(L"ManaDepleted");
}

void GPlayerController::Fire()	{ playerFSMComponent->InputKey(InputEvent::Fire); }
void GPlayerController::Water() { playerFSMComponent->InputKey(InputEvent::Water); }
void GPlayerController::Light() { playerFSMComponent->InputKey(InputEvent::Light); }
void GPlayerController::Dark()	{ playerFSMComponent->InputKey(InputEvent::Dark); }
void GPlayerController::Attack() { 
	//Math::Vector2 destPos = GetWorld()->ScreenToWorldPoint({ Mouse::curMousePosition.x, Mouse::curMousePosition.y });
	//LOG_WARNING(dbg::text(GetPlayer()->GetLocation().x, ", ", GetPlayer()->GetLocation().y));
	//LOG_INFO(dbg::text(destPos.x, ", ", destPos.y));

	//destPos = GetPlayer()->GetComponent<GCameraComponent>()->GetComponentLocation();
	//LOG_MESSAGE(dbg::text(destPos.x, ", ", destPos.y));
	GetPlayer()->GetComponent<MovementComponent>()->SetSpeed(0.f);


	playerFSMComponent->InputKey(InputEvent::Attack); 
}
void GPlayerController::Move() { playerFSMComponent->InputKey(InputEvent::Move); }

void GPlayerController::Memorize() {
	playerFSMComponent->InputKey(InputEvent::Memorize);
	if (CheckMemorize())
		OnMemorize->Emit();
	else
		OffMemorize->Emit();
}

void GPlayerController::Teleport() { playerFSMComponent->InputKey(InputEvent::Teleport); 
		OnFlash->Emit();
}

void GPlayerController::Cancellation() { playerFSMComponent->InputKey(InputEvent::Cancellation); }

void GPlayerController::DisfellEvent(const DisFellEvent* const _event)
{
	if (bElementalMaster)
		return;
	if (targetSkill == nullptr && !_event->GetBossSkillDieFlag())
	{
		targetSkill = _event->GetBossSkillActor();
		playerFSMComponent->SetNextState(L"PlayerDisfell");
		
		for (int i = 0; i < _event->GetBossSkillActor()->disfellCommandCount; i++)
		{
			OnBeginDisfell->Emit(i, _event->GetBossSkillActor()->disfellCommand[i]);
		}
	}
	else
	{
		if (targetSkill == _event->GetBossSkillActor() && _event->GetBossSkillDieFlag())
		{
			OnDoingDisfell->Emit(-1, 0);
			playerFSMComponent->SetNextState(L"PlayerIdle");
		}
	}
}

void GPlayerController::Cheat()
{
	GetPlayer()->stat.hpRegenPerSecond = 500.f;
	GetPlayer()->stat.mpRegenPerSecond = 500.f;
	Player::skillUses = 50;
}
