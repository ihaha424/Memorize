#pragma once
#include "ProjectileSkill.h"
class PrismReflection : public ProjectileSkill
{

public:
	PrismReflection(Actor* _owner);
	virtual ~PrismReflection();
	virtual void UseSkill() override;
	virtual void ReflectionIn() override;
};

