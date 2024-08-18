#pragma once
#include "Skill.h"
class ElementalMaster : public Skill
{

	class AnimationState* animState;

public:
	ElementalMaster(Actor* _owner);
	virtual ~ElementalMaster();

	virtual void Initialize() override;
	virtual void UseSkill() override;
	virtual void ReflectionIn() override;
};

