#pragma once
#include "BuffSkill.h"
class Heal : public Skill
{
	float healHP = 0;
public:
	Heal();
	virtual ~Heal();

	virtual void UseSkill() override;
};

