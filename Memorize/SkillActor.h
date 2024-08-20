#pragma once
#include "D2DGameEngine/Actor.h"
#include "Skill.h"
class SkillActor : public Actor
{
public:
	SkilID skill;
public:
	SkillActor(World* _world);
	virtual ~SkillActor();

	void SetSkillID(SkilID _skill) { skill = _skill; }
};

