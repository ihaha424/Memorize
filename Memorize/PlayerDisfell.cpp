#include "PlayerDisfell.h"
#include "GPlayerController.h"
#include "BossSkillActor.h"

void PlayerDisfell::Enter()
{
	//디스펠 이미지?
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
	//취소하면 원래 상태로?
	//Idle상태로? 이게 맞는 듯?>
	owner->SetNextState(L"PlayerIdle");
}
