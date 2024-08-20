#pragma once
#include "Skill.h"
class Purification : public Skill
{
public:
	std::vector<class Actor*> disfellSkills;

	Purification(Actor* _owner);
	virtual ~Purification();

	virtual void Initialize() override;
	virtual void UseSkill() override;
	virtual void ReflectionIn() override;
};

