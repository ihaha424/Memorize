#include "Heal.h"

Heal::Heal(Actor* _owner) : Skill(_owner)
{
}

Heal::~Heal()
{
}

void Heal::UseSkill()
{
	__super::UseSkill();

	player->AddToStat(Stat(healHP, 0,0));
}
