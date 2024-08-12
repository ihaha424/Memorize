#pragma once
#include "Projectile.h"

class PrismReflectionProjectile : public Projectile
{
public:
	PrismReflectionProjectile(World* _world);
	virtual ~PrismReflectionProjectile();

	
	void Initialize() override { __super::Initialize(); }
	virtual void BeginPlay() { __super::BeginPlay(); Inactivate(); }
	void Update(float _dt) override;

};

