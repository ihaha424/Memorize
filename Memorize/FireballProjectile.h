#pragma once
#include "Projectile.h"

class FireballProjectile : public Projectile
{
public:
	FireballProjectile(class World* _world);
	virtual ~FireballProjectile();

	// Projectile을(를) 통해 상속됨
	void Initialize() override { Inactivate(); }
	virtual void BeginPlay() { __super::BeginPlay(); Inactivate(); }
	virtual void Update(float _dt) {__super::Update(_dt);}
};

