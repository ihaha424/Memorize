#include "PlayerAttack.h"
#include "GPlayerController.h"

void PlayerAttack::Enter()
{
}

void PlayerAttack::Update(float _dt)
{
}

void PlayerAttack::Exit()
{
}

void PlayerAttack::Attack()
{
	//GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	
	// 이런 느낌으로 발동 후 idle상태로 진입
	// playerController->StartSkill(SKillList[Player->GetCurSkill]);
}

void PlayerAttack::Memorize()
{
	// if (Memorize 가 비어 있으면)
	//addMagic()
	//else
	//	??
}
