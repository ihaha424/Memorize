#pragma once
#include "RangeSkill.h"
class Meteor : public RangeSkill
{
	class Actor* meteorEffect;
	class MovementComponent* movementComponent;
	float initialHeight = 500.f;
	float fallSpeed = 1500.f;
	Math::Vector2 attackPos;
public:
	Meteor(Actor* _owner);
	virtual ~Meteor();

	virtual void BeginPlay() override;
	virtual void Update(float _dt) override;

	virtual void UseSkill() override;
};

