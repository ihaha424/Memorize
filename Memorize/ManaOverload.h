#pragma once
#include "BuffSkill.h"

class ManaOverload : public BuffSkill
{
	class AnimationState* animState;

public:
	ManaOverload(Actor* _owner);
	virtual ~ManaOverload();

	virtual void Initialize() override;
	virtual void UseSkill() override;
	virtual void ReflectionIn() override;

};

