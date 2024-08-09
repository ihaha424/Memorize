#include "PlayerCasting.h"
#include "D2DGameEngine/FSMComponent.h"

void PlayerCasting::Enter()
{
	//commandList = owner->GetOwner()->GetPlayer()->GetSkillList???

}

void PlayerCasting::Update(float _dt)
{
}

void PlayerCasting::Exit()
{
	commandList.clear();
	index = 0;
}

void PlayerCasting::Fire()
{
	// 큐가 하나씪 줄어드는 방식이면 조금 다르게 해야한다.
	if (commandList[index] == InputEvent::Fire)
	{
		index++;
		if (commandList.size() == index)
		{
			// Success
			owner->SetNextState(L"PlayerAttack");
		}
	}
	else
	{
		//fail
		owner->SetNextState(L"PlayerIdle");
	}
}

void PlayerCasting::Water()
{
}

void PlayerCasting::Light()
{
}

void PlayerCasting::Dark()
{
}
