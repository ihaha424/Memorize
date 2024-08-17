#pragma once
#include "Skill.h"
#include "Player.h"

class BuffSkill : public Skill
{
protected:
	Stat buffStat;
public:
	BuffSkill(Actor* _owner);
	virtual ~BuffSkill();

	virtual void UseSkill() override;

};

