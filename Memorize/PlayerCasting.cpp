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
		//스킬이 잠금해제 되지 않았으면 return 
		if (!curSkill->IsUnlocked())
		{
			if (Player::skillUses >= curSkill->GetConditionCount())
				curSkill->Unlock();
			owner->SetNextState(L"PlayerIdle");
			playerController->InitializeSkillInfo();
			NoCastingState = true;
			return;
		}

		//if(마나 없음 -> return)
		float skillMana = playerController->FindCurSkiil()->GetMana();
		Stat& playerStat = playerController->GetPlayer()->GetStat();

		//심판의 빛 -1은 마나 전체 소모 -> 마나 오버라이즈면 어케해?
		if (skillMana == -1)
		{
			//playerController->FindCurSkiil()->Castring Time = playerStat.mp * 0.02f;
			playerController->FindCurSkiil()->SetMana(playerStat.mp);
			playerStat.mp = 0.f;
		}
		else if (playerController->bManaOverload || playerController->bElementalMaster)
		{
			//마나 오버로드 상태면 mp를 사용하지 않음
		}
		else if (playerStat.mp >= skillMana)
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
	// 큐가 하나씪 줄어드는 방식이면 조금 다르게 해야한다.
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
	// 큐가 하나씪 줄어드는 방식이면 조금 다르게 해야한다.
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
	// 큐가 하나씪 줄어드는 방식이면 조금 다르게 해야한다.
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
	// 큐가 하나씪 줄어드는 방식이면 조금 다르게 해야한다.
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
