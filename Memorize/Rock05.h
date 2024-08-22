#pragma once
#include "D2DGameEngine/Actor.h"
#include "D2DGameEngine/DotTween.h"

class Rock05 : public Actor
{
	DotTween<float>* tweenRotate{ nullptr };
	DotTween<float>* tweenUpAndDown{ nullptr };
	float rotateValue{ 0.f };
	float updownValue{ 0.f };
public:
	Rock05(World* _world);
	virtual ~Rock05();

	virtual void Update(float _dt) override;
};

