#pragma once
#include "BuffSkill.h"
class MPRecovery : public BuffSkill
{
	float mpRecovery = 10.f;
	class AnimationState* animState;
public:
	MPRecovery(Actor* _owner);
	virtual ~MPRecovery();

	virtual void Initialize() override;
	virtual void UseSkill() override;
	virtual void ReflectionIn() override;

};

