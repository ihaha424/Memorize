#pragma once
#include "Projectile.h"
class DarkSphereProjectile : public Projectile
{
public:
	DarkSphereProjectile(World* _world);
	virtual ~DarkSphereProjectile();

	virtual void BeginPlay() { __super::BeginPlay(); Inactivate(); }
	virtual void Update(float _dt) override;
};

