#pragma once
#include "D2DGameEngine/Actor.h"
#include "D2DGameEngine/DotTween.h"

class LeftRock : public Actor
{
	DotTween<float>* tween;
	float value;
public:
	LeftRock(World* _world);
	virtual ~LeftRock();

	virtual void Update(float _dt) override;
};

