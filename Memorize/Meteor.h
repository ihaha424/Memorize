#pragma once
#include "RangeSkill.h"
class Meteor : public RangeSkill
{
	class BitmapComponent* metorEffectComponent;
	class MovementComponent* movementComponent;
	float initialHeight = 500.f;
	float fallSpeed = 500.f;
	Math::Vector2 attackPos;
public:
	Meteor(Actor* _owner);
	virtual ~Meteor();

	virtual void BeginPlay() override;
	virtual void Update(float _dt) override;

	virtual void UseSkill() override;
};

