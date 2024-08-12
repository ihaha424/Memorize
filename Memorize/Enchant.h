#pragma once
#include "BuffSkill.h"
class Enchant : public BuffSkill
{
	float additionalDamage = 0;
	float additionalAttackSpeed = 0;
public:
	Enchant(Actor* _owner);
	virtual ~Enchant();

};