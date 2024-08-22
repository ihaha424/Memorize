#include "PlayerCasting.h"
#include "GPlayerController.h"
#include "Player.h"

void PlayerCasting::Enter()
{
	if (!saveDirtyFlag)
	{

		GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());

		if (playerController->FindCurSkiil() == nullptr)
		{
			owner->SetNextState(L"PlayerIdle");
			playerController->InitializeSkillInfo();
			NoCastingState = true;
			return;
		}
		Skill* curSkill = playerController->FindCurSkiil();
		//��ų�� ������� ���� �ʾ����� return 
		if (!curSkill->IsUnlocked())
		{
			if (Player::skillUses >= curSkill->GetConditionCount())
				curSkill->Unlock();
			owner->SetNextState(L"PlayerIdle");
			playerController->InitializeSkillInfo();
			NoCastingState = true;
			return;
		}

		//if(���� ���� -> return)
		float skillMana = playerController->FindCurSkiil()->GetMana();
		Stat& playerStat = playerController->GetPlayer()->GetStat();

		//������ �� -1�� ���� ��ü �Ҹ� -> ���� ����������� ������?
		if (skillMana == -1)
		{
			//playerController->FindCurSkiil()->Castring Time = playerStat.mp * 0.02f;
			playerController->FindCurSkiil()->SetMana(playerStat.mp);
			playerStat.mp = 0.f;
		}
		else if (playerController->bManaOverload || playerController->bElementalMaster)
		{
			//���� �����ε� ���¸� mp�� ������� ����
		}
		else if (playerStat.mp >= skillMana)
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
			NoCastingState = true;
			return;
		}
	playerController->FindCurSkiil()->SetCommandList(commandList);
	}
	saveDirtyFlag = false;
	NoCastingState = false;
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
	if (!NoCastingState && commandList.size() > index && commandList[index] == InputEvent::Fire)
		index++;
	else
	{
		//fail
		GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
		playerController->InitializeSkillInfo();
		owner->SetNextState(L"PlayerIdle");
		SoundManager::PlayMusic(L"TestResource/Sound/Player/Sound_PlayerCasting02.wav");
	}
}

void PlayerCasting::Water()
{
	// ť�� �ϳ��� �پ��� ����̸� ���� �ٸ��� �ؾ��Ѵ�.
	if (!NoCastingState && commandList.size() > index && commandList[index] == InputEvent::Water)
		index++;
	else
	{
		//fail
		GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
		playerController->InitializeSkillInfo();
		owner->SetNextState(L"PlayerIdle");
		SoundManager::PlayMusic(L"TestResource/Sound/Player/Sound_PlayerCasting02.wav");
	}
}

void PlayerCasting::Light()
{
	// ť�� �ϳ��� �پ��� ����̸� ���� �ٸ��� �ؾ��Ѵ�.
	if (!NoCastingState && commandList.size() > index && commandList[index] == InputEvent::Light)
		index++;
	else
	{
		//fail
		GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
		playerController->InitializeSkillInfo();
		owner->SetNextState(L"PlayerIdle");
		SoundManager::PlayMusic(L"TestResource/Sound/Player/Sound_PlayerCasting02.wav");
	}
}

void PlayerCasting::Dark()
{
	// ť�� �ϳ��� �پ��� ����̸� ���� �ٸ��� �ؾ��Ѵ�.
	if (!NoCastingState && commandList.size() > index && commandList[index] == InputEvent::Dark)
		index++;
	else
	{
		//fail
		GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
		playerController->InitializeSkillInfo();
		owner->SetNextState(L"PlayerIdle");
		SoundManager::PlayMusic(L"TestResource/Sound/Player/Sound_PlayerCasting02.wav");
	}
}

void PlayerCasting::Teleport()
{
	if (NoCastingState)
		return;
	saveDirtyFlag = true;

	__super::Teleport();
}

void PlayerCasting::Cancellation()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	playerController->InitializeSkillInfo();
	owner->SetNextState(L"PlayerIdle");
	SoundManager::PlayMusic(L"TestResource/Sound/Player/Sound_PlayerCasting02.wav");
}
