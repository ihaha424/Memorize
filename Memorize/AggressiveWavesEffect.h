#pragma once
#include "Projectile.h"
class AggressiveWavesEffect : public Projectile
{
public:
	AggressiveWavesEffect(World* _world);
	virtual ~AggressiveWavesEffect();

	void SetAinimation();
};

