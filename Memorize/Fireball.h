#pragma once
#include "ProjectileSkill.h"

class Fireball : public ProjectileSkill
{
	float speed = 2.f;
public:
	Fireball(Actor* _owner);
	virtual ~Fireball();

	virtual void UseSkill() override;
	virtual void ReflectionIn() override;

};

