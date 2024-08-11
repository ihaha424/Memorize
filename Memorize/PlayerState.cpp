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
	// �Ͻ������� SE_FIRE == ST_PROJECTILE �̱�� �ؼ� �ϴ� �Ʒ��� ���� ����� ������,
	// ��������� if�� ����� ǥ�ø� �ؾ� �ϳ� �ͱ�� �ϴ�. ������ ������ ������ ���߿� Ȯ�� or ������ ������ �߻��� ���� ����.
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
	//�⺻ ������ �ٷ� �������� �Ӽ��� Ÿ���� NONE(Default�ʹ� �ٸ�, (Default == END))
	bool CheckStates = playerController->AddSkillInfo(ESkillElement::SE_NONE);
	CheckStates = playerController->AddSkillInfo(ESkillElement::SE_NONE);
	if (CheckStates)
	{
		//ĳ������ �ʿ����� ���� �⺻ ����
		owner->SetNextState(L"PlayerAttack");
	}
}

void PlayerState::Move()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	Math::Vector2 destPos = playerController->GetWorld()->ScreenToWorldPoint({ Mouse::curMousePosition.x, Mouse::curMousePosition.y });
	Math::Vector2 direction = destPos - playerController->GetPlayer()->GetLocation();
	direction.Normalize();
	playerController->GetPlayer()->GetComponent<MovementComponent>()->SetDirection(direction);
	playerController->GetPlayer()->GetComponent<MovementComponent>()->SetSpeed(500.0f);
}

void PlayerState::Memorize()
{
	// if()// �÷��̾ ������ ������ ������...)
	//owner->SetNextState(L"PlayerAttack");
	// ������ ��Ͼ���
}
