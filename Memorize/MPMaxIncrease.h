#pragma once
#include "BuffSkill.h"
class MPMaxIncrease : public BuffSkill
{
	float additionalMP = 0;
public:
	MPMaxIncrease(Actor* _owner);
	virtual ~MPMaxIncrease();

	virtual void UseSkill() override;
	virtual void ReflectionIn() override;

};

