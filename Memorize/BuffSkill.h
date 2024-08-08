#pragma once
#include "Skill.h"
#include "Player.h"

class BuffSkill : public Skill
{
protected:
	Stat buffStat;
	float buffDuration;
	float elapsedTime = 0.f;
public:
	BuffSkill();
	virtual ~BuffSkill();

	virtual void Update(float _dt) override;

	virtual void UseSkill() override;

};

