#include "PlayerDisfell.h"
#include "GPlayerController.h"
#include "BossSkillActor.h"

void PlayerDisfell::Enter()
{
	//���� �̹���?
	//targetSkill = static_cast<GPlayerController*>(owner->GetOwner())->GetTargetSKill();
}

void PlayerDisfell::Update(float _dt)
{
}

void PlayerDisfell::Exit()
{
	targetSkill = nullptr;
}

void PlayerDisfell::Fire()
{
	targetSkill->Disfell(InputEvent::Fire);
}

void PlayerDisfell::Water()
{
}

void PlayerDisfell::Light()
{
}

void PlayerDisfell::Dark()
{
}

void PlayerDisfell::Cancellation()
{
	//����ϸ� ���� ���·�?
	//Idle���·�? �̰� �´� ��?>
	owner->SetNextState(L"PlayerIdle");
}
