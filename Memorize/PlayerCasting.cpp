#include "PlayerCasting.h"
#include "GPlayerController.h"
#include "Player.h"

void PlayerCasting::Enter()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());

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
	// ť�� �ϳ��� �پ��� ����̸� ���� �ٸ��� �ؾ��Ѵ�.
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
	// ť�� �ϳ��� �پ��� ����̸� ���� �ٸ��� �ؾ��Ѵ�.
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
	// ť�� �ϳ��� �پ��� ����̸� ���� �ٸ��� �ؾ��Ѵ�.
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
	// ť�� �ϳ��� �پ��� ����̸� ���� �ٸ��� �ؾ��Ѵ�.
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

