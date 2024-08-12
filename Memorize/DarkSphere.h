#pragma once
#include "ProjectileSkill.h"
class DarkSphere : public ProjectileSkill
{
public:
	DarkSphere(Actor* _owner);
	virtual ~DarkSphere();
	virtual void UseSkill() override;
	virtual void ReflectionIn() override;
};

