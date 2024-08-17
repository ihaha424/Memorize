#pragma once
#include "BuffSkill.h"
class MPMaxIncrease : public BuffSkill
{
	float additionalMP = 10;
	class AnimationState* animState;
public:
	MPMaxIncrease(Actor* _owner);
	virtual ~MPMaxIncrease();

	virtual void Initialize() override;
	virtual void UseSkill() override;
	virtual void ReflectionIn() override;

};

