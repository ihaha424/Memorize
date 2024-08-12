#include "PlayerCasting.h"
#include "GPlayerController.h"
#include "Player.h"

void PlayerCasting::Enter()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());

	//if(마나 없음 -> return)
	float skillMana = playerController->FindCurSkiil()->GetMana();
	Stat& playerStat = playerController->GetPlayer()->GetStat();

	//심판의 빛 -1은 마나 전체 소모 -> 마나 오버라이즈면 어케해?
	if (skillMana == -1)
	{
		//playerController->FindCurSkiil()->Castring Time = playerStat.mp * 0.02f;
		playerStat.mp = 0.f;
	}

	if (playerStat.mp >= skillMana)
	{
		playerStat.mp -= skillMana;
		owner->SetNextState(L"PlayerCasting");
	}
	else
	{
		playerController->InitializeSkillInfo();
		owner->SetNextState(L"PlayerIdle");
		return;
	}

	playerController->FindCurSkiil()->SetCommandList(commandList);
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

