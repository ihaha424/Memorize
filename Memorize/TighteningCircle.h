#pragma once
#include "RangeSkill.h"
class TighteningCircle : public RangeSkill
{
	class TighteningCircleEffect* circleEffects[3];
	int nowUsingIndex = 0;
public:
	TighteningCircle(Actor* _owner);
	virtual ~TighteningCircle();

	virtual void UseSkill() override;
	virtual void ReflectionIn() override;
};

