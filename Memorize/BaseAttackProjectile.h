#pragma once
#include "Projectile.h"
class BaseAttackProjectile : public Projectile
{
public:
	BaseAttackProjectile(class World* _world);
	virtual ~BaseAttackProjectile();

	virtual void OnBeginOverlap(Actor* other) override;

	void Initialize() override { Inactivate(); }
	virtual void BeginPlay() { __super::BeginPlay(); Inactivate(); }
	virtual void Update(float _dt) { __super::Update(_dt); }

};

