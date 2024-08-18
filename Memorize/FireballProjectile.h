#pragma once
#include "Projectile.h"

class FireballProjectile : public Projectile
{
public:
	FireballProjectile(class World* _world);
	virtual ~FireballProjectile();

	void Initialize() override { __super::Initialize(); Inactivate(); }
	virtual void BeginPlay() { __super::BeginPlay(); Inactivate(); }
	virtual void Update(float _dt) {__super::Update(_dt);}
};

