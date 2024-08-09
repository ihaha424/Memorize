#include "PlayerFSMComponent.h"

void PlayerFSMComponent::BeginPlay()
{
	__super::BeginPlay();
	CreateState<PlayerIdle>(L"PlayerIdle");
	CreateState<PlayerMove>(L"PlayerMove");
	CreateState<PlayerCasting>(L"PlayerCasting");
	CreateState<PlayerAttack>(L"PlayerAttack");
//	CreateState<PlayerBlinking>(L"PlayerBlinking");
	SetNextState(L"PlayerIdle");
}

void PlayerFSMComponent::InputKey(int _key)
{
	PlayerState* ActionState = static_cast<PlayerState*>(currState);
	switch (_key)
	{
	case InputEvent::Fire:
		ActionState->Fire();
		break;
	case InputEvent::Water:
		ActionState->Water();
		break;
	case InputEvent::Light:
		ActionState->Light();
		break;
	case InputEvent::Dark:
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
default:
		break;
	}
}
