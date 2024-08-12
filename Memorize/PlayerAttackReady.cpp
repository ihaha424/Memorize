#include "PlayerAttackReady.h"
#include "GPlayerController.h"
#include "Skill.h"

void PlayerAttackReady::Enter()
{
	std::cout << "AttackReady" << std::endl;
}

void PlayerAttackReady::Update(float _dt)
{

}

void PlayerAttackReady::Exit()
{
}

void PlayerAttackReady::Attack()
{
	owner->SetNextState(L"PlayerAttack");
}

void PlayerAttackReady::Memorize()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	if (playerController->CheckMemorize())
	{
		playerController->SwapMemorize();
		owner->SetNextState(L"PlayerAttackReady");
	}
}

