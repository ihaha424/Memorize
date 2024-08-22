#include "PlayerFSMComponent.h"
#include "../D2DGameEngine/SoundManager.h"

PlayerFSMComponent::PlayerFSMComponent(Actor* _owner)
	: FSMComponent(_owner)
{
	SetTickProperties(TICK_UPDATE);
	CreateState<PlayerIdle>(L"PlayerIdle");
	//CreateState<PlayerMove>(L"PlayerMove");
	CreateState<PlayerCasting>(L"PlayerCasting");
	CreateState<PlayerAttackReady>(L"PlayerAttackReady");
	CreateState<PlayerAttack>(L"PlayerAttack");
	CreateState<PlayerBlinking>(L"PlayerBlinking");
	CreateState<PlayerDisfell>(L"PlayerDisfell");
	SetNextState(L"PlayerIdle");
}

void PlayerFSMComponent::BeginPlay()
{
	__super::BeginPlay();
}

void PlayerFSMComponent::InputKey(int _key)
{
	if (OneTImeOneInput)
		return;
	PlayerState* ActionState = static_cast<PlayerState*>(currState);
	switch (_key)
	{
	case InputEvent::Fire:
		SoundManager::PlayMusic(L"TestResource/Sound/UI/Sound_UI_Button01.wav");
		ActionState->Fire();
		break;
	case InputEvent::Water:
		SoundManager::PlayMusic(L"TestResource/Sound/UI/Sound_UI_Button02.wav");
		ActionState->Water();
		break;
	case InputEvent::Light:
		SoundManager::PlayMusic(L"TestResource/Sound/UI/Sound_UI_Button03.wav");
		ActionState->Light();
		break;
	case InputEvent::Dark:
		SoundManager::PlayMusic(L"TestResource/Sound/UI/Sound_UI_Button04.wav");
		ActionState->Dark();
		break;
	case InputEvent::Attack:
		ActionState->Attack();
		break;
	case InputEvent::Move:
		ActionState->Move();
		break;
	case InputEvent::Memorize:
		ActionState->Memorize();
		break;
	case InputEvent::Teleport:
		ActionState->Teleport();
		break;
	case InputEvent::Cancellation:
		ActionState->Cancellation();
		break;
default:
		break;
	}
	OneTImeOneInput = true;
}
