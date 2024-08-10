#include "PlayerCasting.h"
#include "GPlayerController.h"

void PlayerCasting::Enter()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	playerController->FindCurSkiil()->SetCommandList(commandList);

	//마나 소모, 실패시 바로 이전 상태로
}

void PlayerCasting::Update(float _dt)
{
	if (commandList.size() == index)
	{
		// Success
		owner->SetNextState(L"PlayerAttackReady");
	}
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
		index++;
	else
	{
		//fail
		GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
		playerController->InitializeSkill();
		owner->SetNextState(L"PlayerIdle");
	}
}

void PlayerCasting::Water()
{
	// 큐가 하나씪 줄어드는 방식이면 조금 다르게 해야한다.
	if (commandList[index] == InputEvent::Water)
		index++;
	else
	{
		//fail
		GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
		playerController->InitializeSkill();
		owner->SetNextState(L"PlayerIdle");
	}
}

void PlayerCasting::Light()
{
	// 큐가 하나씪 줄어드는 방식이면 조금 다르게 해야한다.
	if (commandList[index] == InputEvent::Light)
		index++;
	else
	{
		//fail
		GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
		playerController->InitializeSkill();
		owner->SetNextState(L"PlayerIdle");
	}
}

void PlayerCasting::Dark()
{
	// 큐가 하나씪 줄어드는 방식이면 조금 다르게 해야한다.
	if (commandList[index] == InputEvent::Dark)
		index++;
	else
	{
		//fail
		GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
		playerController->InitializeSkill();
		owner->SetNextState(L"PlayerIdle");
	}
}

