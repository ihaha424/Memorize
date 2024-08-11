#include "Heal.h"

Heal::Heal(Actor* _owner) : Skill(_owner)
{
	SetID(ST_BUFF, SE_WATER);
}

Heal::~Heal()
{
}

void Heal::UseSkill()
{
	__super::UseSkill();

	player->AddToStat(Stat(healHP, 0,0));
}
