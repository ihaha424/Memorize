#pragma once
#include "Skill.h"
#include "Player.h"

class BuffSkill : public Skill
{
	Stat buffStat;

public:
	BuffSkill();
	virtual ~BuffSkill();

	virtual void UseSkill() override;
};

