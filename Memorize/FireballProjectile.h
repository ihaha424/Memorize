#pragma once
#include "Projectile.h"

class FireballProjectile : public Projectile
{
public:
	FireballProjectile(class World* _world);
	virtual ~FireballProjectile();

	virtual void OnBeginOverlap(Actor* other) override;

	void Initialize() override { Inactivate(); }
	virtual void BeginPlay() { __super::BeginPlay(); Inactivate(); }
	virtual void Update(float _dt) {__super::Update(_dt);}
};

