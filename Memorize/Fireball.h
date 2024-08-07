#pragma once
#include "ProjectileSkill.h"

class Fireball : public ProjectileSkill
{
	int maxCount = 1;
	int nowCount = 1;
	float speed = 2.f;
public:
	Fireball(class World* _world);
	virtual ~Fireball();

	virtual void BeginPlay() override;
	virtual void UseSkill() override;

};

