#pragma once
#include "Projectile.h"
class AggressiveWavesEffect : public Projectile
{
public:
	AggressiveWavesEffect(World* _world);
	virtual ~AggressiveWavesEffect();

	virtual void BeginPlay() override;
	virtual void Update(float _dt) override;

};
