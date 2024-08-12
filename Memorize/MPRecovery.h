#pragma once
#include "BuffSkill.h"
class MPRecovery : public BuffSkill
{
	float mpRecovery = 0.f;
public:
	MPRecovery(Actor* _owner);
	virtual ~MPRecovery();
};

