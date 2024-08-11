#include "PlayerCasting.h"
#include "GPlayerController.h"

void PlayerCasting::Enter()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	playerController->FindCurSkiil()->SetCommandList(commandList);

	//���� �Ҹ�, ���н� �ٷ� ���� ���·�
}

void PlayerCasting::Update(float _dt)
{
	if (commandList.size() == index)
	{
		if (commandList.size() == index)
		{
			// Success

			//owner->SetNextState(L"PlayerAttackReady");
			owner->SetNextState(L"PlayerAttack");
		}
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

