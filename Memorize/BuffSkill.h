#pragma once
#include "Skill.h"
#include "Player.h"

class BuffSkill : public Skill
{
	Stat buffStat;

public:
	BuffSkill(class World* _world, std::wstring _id);
	virtual ~BuffSkill();

	virtual void UseSkill() override;
};

