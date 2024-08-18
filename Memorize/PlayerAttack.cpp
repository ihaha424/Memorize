#include "PlayerAttack.h"
#include "GPlayerController.h"
#include "Skill.h"

void PlayerAttack::Enter()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	playerController->FindCurSkiil()->UseSkill();
	skillDelay = playerController->FindCurSkiil()->GetCastingTime();
	playerController->bNowAttacking = true;
}

void PlayerAttack::Update(float _dt)
{
	//스킬 정보에 따라 애니메이션이 끝나면 Idle로
	// if(finish Animi)
	//	owner->SetNextState(L"Idle");
	//  or
	// coolTimeFunction->Animiation-end??

	skillDelay -= _dt;
	if (skillDelay < 0.f)
	{
		owner->SetNextState(L"PlayerIdle");
	}
}

void PlayerAttack::Exit()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	playerController->InitializeSkillInfo();
}
