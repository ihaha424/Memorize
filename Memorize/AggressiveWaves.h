#pragma once
#include "RangeSkill.h"
class AggressiveWaves : public RangeSkill
{
	std::vector<class AggressiveWavesEffect*> waveEffects;

	float angles[4] = { -30, 30, 150, 210 };
	float SetAngles[4] = { -45, 45, 135, 225 };
	float speed = 600.f;
	int curIndex = 0;
public:
	AggressiveWaves(Actor* _owner);
	virtual ~AggressiveWaves();

	virtual void UseSkill() override;
	virtual void ReflectionIn() override;
};

