#pragma once
#include "Skill.h"
class Purification : public Skill
{
	class AnimationState* animState;

public:
	Purification(Actor* _owner);
	virtual ~Purification();

	virtual void Initialize() override;
	virtual void UseSkill() override;
	virtual void ReflectionIn() override;
};

