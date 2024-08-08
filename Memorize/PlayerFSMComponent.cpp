#include "PlayerFSMComponent.h"

void PlayerFSMComponent::BeginPlay()
{
	CreateState<PlayerIdle>(L"PlayerIdle");
//	CreateState<PlayerCasting>(L"PlayerCasting");
//	CreateState<PlayerAttack>(L"PlayerAttack");
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
	case InputEvent::Water:
		ActionState->Water();
	case InputEvent::Light:
		ActionState->Light();
	case InputEvent::Dark:
		ActionState->Dark();
	case InputEvent::Attack:
		ActionState->Attack();
	case InputEvent::Move:
		ActionState->Move();
	default:
		break;
	}
}
