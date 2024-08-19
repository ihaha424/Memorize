#pragma once
#include "Projectile.h"

class PrismReflectionProjectile : public Projectile
{
public:
	PrismReflectionProjectile(World* _world);
	virtual ~PrismReflectionProjectile();

	
	virtual void BeginPlay() { __super::BeginPlay(); Inactivate(); }
	void Update(float _dt) override;

};

