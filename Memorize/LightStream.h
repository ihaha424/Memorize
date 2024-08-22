#pragma once
#include "RangeSkill.h"
class LightStream : public RangeSkill
{
	class LightStreamEffect* lightStreamEffect[4];
	int	curIndex = 0;
	int maxIndex = 4;
	class AnimationEffect* orbEffect;
public:
	LightStream(Actor* _owner);
	virtual ~LightStream();

	virtual void UseSkill() override;
	virtual void ReflectionIn() override;
};

