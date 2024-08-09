#pragma once
#include "Projectile.h"

class PrismReflectionProjectile : public Projectile
{
public:
	PrismReflectionProjectile(World* _world);
	virtual ~PrismReflectionProjectile();

	
	void Initialize() override {}
	void Update() {}

};

