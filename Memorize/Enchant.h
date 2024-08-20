#pragma once
#include "BuffSkill.h"
class Enchant : public BuffSkill
{
	float additionalDamage = 5.f;
	float additionalAttackSpeed = 0.1f;
public:
	Enchant(Actor* _owner);
	virtual ~Enchant();

	virtual void Initialize() override;
	virtual void UseSkill() override;
	virtual void ReflectionIn() override;
};