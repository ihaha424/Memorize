#pragma once
#include "ProjectileSkill.h"

class Fireball : public ProjectileSkill
{
	int maxCount = 1;
	int nowCount = 1;
	float speed = 10.f;
public:
	Fireball(class World* _world);
	virtual ~Fireball();

	virtual void UseSkill() override;

};

