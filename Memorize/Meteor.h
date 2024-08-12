#pragma once
#include "RangeSkill.h"
class Meteor : public RangeSkill
{
	class MeteorEffect* meteorEffect;
	float initialHeight = 500.f;
	float fallSpeed = 1500.f;
public:
	Meteor(Actor* _owner);
	virtual ~Meteor();

	virtual void UseSkill() override;
	virtual void ReflectionIn() override;
};

