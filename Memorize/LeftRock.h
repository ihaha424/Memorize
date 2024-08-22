#pragma once
#include "D2DGameEngine/Actor.h"
#include "D2DGameEngine/DotTween.h"

class LeftRock : public Actor
{
	DotTween<float>* tweenRotate;
	DotTween<float>* tweenUpAndDown;
	float rotateValue;
	float updownValue;
public:
	LeftRock(World* _world);
	virtual ~LeftRock();

	virtual void Update(float _dt) override;
};

