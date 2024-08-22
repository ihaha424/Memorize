#pragma once
#include "BuffSkill.h"
class Heal : public BuffSkill
{
	float healHP = 50.f;
public:
	Heal(Actor* _owner);
	virtual ~Heal();

	virtual void Initialize() override;
	virtual void UseSkill() override;
	virtual void ReflectionIn() override;
};

 