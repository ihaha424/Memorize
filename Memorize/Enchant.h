#pragma once
#include "BuffSkill.h"
class Enchant : public BuffSkill
{
	float additionalDamage = 5.f;
	float additionalAttackSpeed = 0.1f;
	class AnimationState* animState;
public:
	Enchant(Actor* _owner);
	virtual ~Enchant();

	virtual void UseSkill() override;
	virtual void ReflectionIn() override;
};