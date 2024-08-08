#pragma once
#include "Skill.h"
class RangeSkill : public Skill
{
protected:
	float range;

public:
	RangeSkill(Actor* _owner);
	virtual ~RangeSkill();

	float GetRange() { return range; }

};

