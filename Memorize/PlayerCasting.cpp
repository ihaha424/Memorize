#include "PlayerCasting.h"
#include "GPlayerController.h"
#include "Player.h"

void PlayerCasting::Enter()
{
	if (!saveDirtyFlag)
	{

		GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());

		//스킬이 잠금해제 되지 않았으면 return 
		if (!playerController->FindCurSkiil()->IsUnlocked())
		{
			owner->SetNextState(L"PlayerIdle");
			playerController->InitializeSkillInfo();
			return;
		}

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
			//owner->SetNextState(L"PlayerCasting");
		}
		else
		{
			//마나 부족
			playerController->OnManaDepleted();
			playerController->InitializeSkillInfo();
			owner->SetNextState(L"PlayerIdle");
			return;
		}
	playerController->FindCurSkiil()->SetCommandList(commandList);
	}
	saveDirtyFlag = false;
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
	if (!saveDirtyFlag)
	{
		commandList.clear();
		index = 1;
	}
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
		playerController->InitializeSkillInfo();
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
		playerController->InitializeSkillInfo();
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
		playerController->InitializeSkillInfo();
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
		playerController->InitializeSkillInfo();
		owner->SetNextState(L"PlayerIdle");
	}
}

void PlayerCasting::Teleport()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	if (playerController->GetPlayer()->bondageFlag)
		return;
	//if(마나체크)

	saveDirtyFlag = true;
	owner->SetNextState(L"PlayerBlinking");
}

void PlayerCasting::Cancellation()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	playerController->InitializeSkillInfo();
	owner->SetNextState(L"PlayerIdle");
}
