#include "ElementalMasterComponent.h"
#include "Player.h"
#include "GPlayerController.h"
#include "PlayerFSMComponent.h"
#include "ProjectileSkill.h"

ElementalMasterComponent::ElementalMasterComponent(Actor* _owner) : IComponent(_owner)
{
	SetTickProperties(TICK_UPDATE);
}

ElementalMasterComponent::~ElementalMasterComponent()
{
}

void ElementalMasterComponent::BeginPlay()
{
	
}

void ElementalMasterComponent::Update(float _dt)
{
	controller = static_cast<GPlayerController*>(GetOwner());
	
	if (controller->bElementalMaster == false)
	{
		bSkillReady = false;

		SetStatus(OS_INACTIVE);
		return;
	}

	if (bSkillReady && controller->bNowAttacking)
	{
		Skill* nowSkill = controller->FindCurSkiil();
		if (nowSkill == nullptr || static_cast<ProjectileSkill*>(nowSkill)->bEnd == true)
		{
			bSkillReady = false;
			controller->bNowAttacking = false;
			controller->InitializeSkillInfo();
		}
		return;
	}


	if (!bSkillReady )
	{
		controller->playerFSMComponent->SetNextState(L"PlayerCasting");
		controller->SetRandomSkillReady();
		if (controller->playerFSMComponent->GetCurState() == L"PlayerCasting")
		{
			PlayerCasting* state = static_cast<PlayerCasting*>(controller->playerFSMComponent->GetCurStateClass());
			state->index = state->commandList.size();
			bSkillReady = true;
		}

	}

}
