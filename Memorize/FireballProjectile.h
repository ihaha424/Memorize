#pragma once
#include "Projectile.h"

class FireballProjectile : public Projectile
{
public:
	FireballProjectile(class World* _world);
	virtual ~FireballProjectile();

	// Projectile��(��) ���� ��ӵ�
	void Initialize() override {}
	void Update() {}
};
