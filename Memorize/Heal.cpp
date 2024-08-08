#include "Heal.h"

Heal::Heal()
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
