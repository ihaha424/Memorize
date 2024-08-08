#pragma once
#include "ProjectileSkill.h"

class Fireball : public ProjectileSkill
{
	int maxCount = 1;
	int nowCount = 1;
	float speed = 2.f;
public:
	Fireball(Actor* _owner);
	virtual ~Fireball();

	virtual void BeginPlay() override;
	virtual void UseSkill() override;

};

