#pragma once
#include "Projectile.h"

class FireballProjectile : public Projectile
{
public:
	FireballProjectile(class World* _world);
	virtual ~FireballProjectile();

	// Projectile을(를) 통해 상속됨
	void Initialize() override {}
};

