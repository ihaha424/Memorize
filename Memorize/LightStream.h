#pragma once
#include "RangeSkill.h"
class LightStream : public RangeSkill
{
	class LightStreamEffect* lightStreamEffect;
	class AnimationEffect* orbEffect;
public:
	LightStream(Actor* _owner);
	virtual ~LightStream();

	virtual void UseSkill() override;
	virtual void ReflectionIn() override;
};

