#pragma once
#include "RangeSkill.h"
class AggressiveWaves : public RangeSkill
{
	std::vector<class AggressiveWavesEffect*> waveEffects;

	float angles[4] = { -30, 30, 150, 210};
	float speed = 300.f;
public:
	AggressiveWaves(Actor* _owner);
	virtual ~AggressiveWaves();

	virtual void UseSkill() override;
	virtual void ReflectionIn() override;
};

