#pragma once
#include "D2DGameEngine/Actor.h"
#include "D2DGameEngine/DotTween.h"

class Title : public Actor
{
	DotTween<float>* tweenUpAndDown{ nullptr };
	float updownValue{ 0.f };
public:
	Title(World* _world);
	virtual ~Title();

	virtual void Update(float _dt) override;
};

