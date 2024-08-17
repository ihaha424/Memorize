#pragma once
#include "BuffSkill.h"
class Heal : public BuffSkill
{
	float healHP = 20.f;
	class AnimationState* animState; 

public:
	Heal(Actor* _owner);
	virtual ~Heal();

	virtual void Initialize() override;
	virtual void UseSkill() override;
	virtual void ReflectionIn() override;
};

