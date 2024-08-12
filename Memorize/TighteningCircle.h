#pragma once
#include "RangeSkill.h"
class TighteningCircle : public RangeSkill
{
	class TighteningCircleEffect* circleEffect = nullptr;
public:
	TighteningCircle(Actor* _owner);
	virtual ~TighteningCircle();

	virtual void UseSkill() override;
	virtual void ReflectionIn() override;
};

