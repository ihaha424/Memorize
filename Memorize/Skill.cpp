#include "Skill.h"
#include "Player.h"
#include "GPlayerController.h"

Skill::Skill(Actor* _owner) : IComponent(_owner)
{
}

Skill::~Skill()
{
}

void Skill::SetPlayer(GPlayerController* _pc)
{
	controller = _pc;
	player = controller->GetPlayer();
	if (conditionCount <= player->GetSkillUses())
	{
		bUnlocked = true;
	}
}

void Skill::UseSkill()
{
	player->AddSkillUses();
	if (conditionCount <= player->GetSkillUses())
	{
		bUnlocked = true;
	}

}