#include "PlayerCasting.h"
#include "GPlayerController.h"
#include "Player.h"

void PlayerCasting::Enter()
{
	if (!saveDirtyFlag)
	{

		GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());

		//��ų�� ������� ���� �ʾ����� return 
		if (!playerController->FindCurSkiil()->IsUnlocked())
		{
			owner->SetNextState(L"PlayerIdle");
			playerController->InitializeSkillInfo();
			return;
		}

		//if(���� ���� -> return)
		float skillMana = playerController->FindCurSkiil()->GetMana();
		Stat& playerStat = playerController->GetPlayer()->GetStat();

		//������ �� -1�� ���� ��ü �Ҹ� -> ���� ����������� ������?
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
			//���� ����
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
	// ť�� �ϳ��� �پ��� ����̸� ���� �ٸ��� �ؾ��Ѵ�.
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
	// ť�� �ϳ��� �پ��� ����̸� ���� �ٸ��� �ؾ��Ѵ�.
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
	// ť�� �ϳ��� �پ��� ����̸� ���� �ٸ��� �ؾ��Ѵ�.
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
	// ť�� �ϳ��� �پ��� ����̸� ���� �ٸ��� �ؾ��Ѵ�.
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
	//if(����üũ)

	saveDirtyFlag = true;
	owner->SetNextState(L"PlayerBlinking");
}

void PlayerCasting::Cancellation()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	playerController->InitializeSkillInfo();
	owner->SetNextState(L"PlayerIdle");
}
