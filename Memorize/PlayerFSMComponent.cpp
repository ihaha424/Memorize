#include "PlayerFSMComponent.h"

void PlayerFSMComponent::BeginPlay()
{
	CreateState<PlayerIdle>(L"PlayerIdle");
//	CreateState<PlayerCasting>(L"PlayerCasting");
//	CreateState<PlayerAttack>(L"PlayerAttack");
//	CreateState<PlayerBlinking>(L"PlayerBlinking");
}

void PlayerFSMComponent::BeginPlay()
{
}

void PlayerFSMComponent::InputKey(int _key)
{
	PlayerState* ActionState = static_cast<PlayerState*>(currState);
	switch (_key)
	{
	case 'Q':
		ActionState->Q();
	case 'W':
		ActionState->W();
	case 'E':
		ActionState->E();
	case 'R':
		ActionState->R();
	case 1:
		ActionState->Left();
	case 0:
		ActionState->Right();
	default:
		break;
	}
}
