#pragma once
#include "ProjectileSkill.h"
class BasicAttack : public ProjectileSkill
{
public:
	BasicAttack(Actor* _owner);
	virtual ~BasicAttack();

	virtual void UseSkill() override;

	virtual void ReflectionIn() {};
	virtual void ReflectionOut() {}
};

