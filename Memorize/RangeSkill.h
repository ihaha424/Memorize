#pragma once
#include "Skill.h"
class RangeSkill : public Skill
{
protected:
	float range;

public:
	RangeSkill();
	virtual ~RangeSkill();

	float GetRange() { return range; }

};

