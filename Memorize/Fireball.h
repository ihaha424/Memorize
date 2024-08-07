#pragma once
#include "ProjectileSkill.h"

class Fireball : public ProjectileSkill
{
	float speed = 10.f;
public:
	Fireball(class World* _world);
	virtual ~Fireball();

	virtual void UseSkill() override;

};

