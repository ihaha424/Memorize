#include "PlayerDisfell.h"
#include "GPlayerController.h"
#include "BossSkillActor.h"

void PlayerDisfell::Enter()
{
	//���� �̹���?
	std::cout << "PlayerDisfell Enter\n";
}

void PlayerDisfell::Update(float _dt)
{

}

void PlayerDisfell::Exit()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	playerController->targetSkill = nullptr;
}

void PlayerDisfell::Fire()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	if (!playerController->targetSkill->Disfell(InputEvent::Fire, playerController))
	{
		owner->SetNextState(L"PlayerIdle");
	}
}

void PlayerDisfell::Water()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	if (!playerController->targetSkill->Disfell(InputEvent::Water, playerController))
	{
		owner->SetNextState(L"PlayerIdle");
	}
}

void PlayerDisfell::Light()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	if (!playerController->targetSkill->Disfell(InputEvent::Light, playerController))
	{
		owner->SetNextState(L"PlayerIdle");
	}
}

void PlayerDisfell::Dark()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	if (!playerController->targetSkill->Disfell(InputEvent::Dark, playerController))
	{
		owner->SetNextState(L"PlayerIdle");
	}
}

void PlayerDisfell::Cancellation()
{
	//����ϸ� ���� ���·�?
	//Idle���·�? �̰� �´� ��?>
	owner->SetNextState(L"PlayerIdle");
}
