#include "Enchant.h"

Enchant::Enchant(Actor* _owner) : BuffSkill(_owner)
{
	buffStat = Stat(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0, 
		additionalDamage, additionalAttackSpeed);
}

Enchant::~Enchant()
{
}
