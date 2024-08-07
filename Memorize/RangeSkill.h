#pragma once
#include "Skill.h"
class RangeSkill : public Skill
{
protected:
	float range;

public:
	RangeSkill(class World* _world, std::wstring _id);
	virtual ~RangeSkill();

	float GetRange() { return range; }

};

