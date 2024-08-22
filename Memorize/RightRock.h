#pragma once
#include "D2DGameEngine/Actor.h"

class RightRock : public Actor
{
public:
	RightRock(World* _world);
	virtual ~RightRock();

	virtual void Update(float _dt) override;
};

