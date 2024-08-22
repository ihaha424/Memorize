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
	if (conditionCount <= Player::skillUses)
	{
		bUnlocked = true;
	}
}

void Skill::UseSkill()
{
	Player::skillUses++;
	std::cout << Player::skillUses << std::endl;

}