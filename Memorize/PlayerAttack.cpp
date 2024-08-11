#include "PlayerAttack.h"
#include "GPlayerController.h"
#include "Skill.h"

void PlayerAttack::Enter()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	playerController->FindCurSkiil()->UseSkill();
}

void PlayerAttack::Update(float _dt)
{
	//��ų ������ ���� �ִϸ��̼��� ������ Idle��
	// if(finish Animi)
	//	owner->SetNextState(L"Idle");
	//  or
	// coolTimeFunction->Animiation-end??

	static float temptime = 0.f;
	temptime += _dt;
	if (temptime > 3.f)
	{
		owner->SetNextState(L"PlayerIdle");
	}
}

void PlayerAttack::Exit()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	playerController->InitializeSkillInfo();
}

