#include "PlayerState.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Mouse.h"
#include "MovementComponent.h"
#include "GPlayerController.h"
#include "Player.h"


#include "Fireball.h"

void PlayerState::Fire()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	// 암시적으로 SE_FIRE == ST_PROJECTILE 이기는 해서 일단 아래와 같이 사용은 했으나,
	// 명시적으로 if로 나누어서 표시를 해야 하나 싶기는 하다. 로직상 문제는 없지만 나중에 확장 or 수정시 문제가 발생할 수도 있음.
	bool CheckStates = playerController->AddSkillInfo(ESkillElement::SE_FIRE);
	if (CheckStates)
	{
		owner->SetNextState(L"PlayerCasting");
	}
}

void PlayerState::Water()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	bool CheckStates = playerController->AddSkillInfo(ESkillElement::SE_WATER);
	if (CheckStates)
	{
		owner->SetNextState(L"PlayerCasting");
	}
}

void PlayerState::Light()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	bool CheckStates = playerController->AddSkillInfo(ESkillElement::SE_LIGHT);
	if (CheckStates)
	{
		owner->SetNextState(L"PlayerCasting");
	}
}

void PlayerState::Dark()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	bool CheckStates = playerController->AddSkillInfo(ESkillElement::SE_DARKNESS);
	if (CheckStates)
	{
		owner->SetNextState(L"PlayerCasting");
	}
}

void PlayerState::Attack()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	if (playerController->bElementalMaster)
		return;
	if (playerController->GetPlayer()->basicAttackTime > 0.f)
		return;
	//기본 공격은 바로 나감으로 속성과 타입은 NONE(Default와는 다름, (Default == END))
	bool CheckStates = playerController->AddSkillInfo(ESkillElement::SE_NONE);
	if (CheckStates)
	{
		playerController->InitializeSkillInfo();
		playerController->AddSkillInfo(ESkillElement::SE_NONE);
	}
	CheckStates = playerController->AddSkillInfo(ESkillType::ST_NONE);
	if (CheckStates)
	{
		//캐스팅이 필요하지 않은 기본 공격
		owner->SetNextState(L"PlayerAttack");
		playerController->GetPlayer()->basicAttackTime = playerController->GetPlayer()->stat.defaultAttackSpeed;
	}
}

void PlayerState::Move()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	if (playerController->GetPlayer()->bondageFlag)
		return;
	Math::Vector2 destPos = playerController->GetWorld()->ScreenToWorldPoint({ Mouse::curMousePosition.x, Mouse::curMousePosition.y });
	playerController->SetDestPos(destPos);
	Math::Vector2 direction = destPos - playerController->GetPlayer()->GetLocation();
	direction.Normalize();
	playerController->GetPlayer()->GetComponent<MovementComponent>()->SetDirection(direction);
	playerController->GetPlayer()->GetComponent<MovementComponent>()->SetSpeed(playerController->GetPlayer()->moveSpeed);
}

void PlayerState::Memorize()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	if (playerController->CheckMemorize())
	{
		playerController->InitializeSkillInfo();
		playerController->SwapMemorize();
		owner->SetNextState(L"PlayerAttackReady");
	}
}

void PlayerState::Teleport()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	if (playerController->GetPlayer()->bondageFlag)
		return;
	//if(마나체크)
	owner->SetNextState(L"PlayerBlinking");
}

void PlayerState::Cancellation()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	playerController->InitializeSkillInfo();
	owner->SetNextState(L"PlayerIdle");
}
