#pragma once
#include "RangeSkill.h"
class Meteor : public RangeSkill
{
	class MeteorEffect* meteorEffect[2];
	float initialHeight = 1000.f;
	float fallSpeed = 1500.f;
	int nowUsingIndex = 0;
public:
	Meteor(Actor* _owner);
	virtual ~Meteor();

	virtual void UseSkill() override;
	virtual void ReflectionIn() override;
};

