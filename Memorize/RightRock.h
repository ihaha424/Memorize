#pragma once
#include "D2DGameEngine/Actor.h"
#include "D2DGameEngine/DotTween.h"

class RightRock : public Actor
{
	DotTween<float>* tweenRotate;
	DotTween<float>* tweenUpAndDown;
	float rotateValue;
	float updownValue;
public:
	RightRock(World* _world);
	virtual ~RightRock();

	virtual void Update(float _dt) override;
};

