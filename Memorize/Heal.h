#pragma once
#include "BuffSkill.h"
class Heal : public Skill
{
	float healHP = 0;
public:
	Heal(Actor* _owner);
	virtual ~Heal();

	virtual void UseSkill() override;
	virtual void ReflectionIn() override;
};

