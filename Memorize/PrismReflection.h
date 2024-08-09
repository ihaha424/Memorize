#pragma once
#include "ProjectileSkill.h"
class PrismReflection : public ProjectileSkill
{

public:
	PrismReflection(Actor* _owner);
	virtual ~PrismReflection();

	virtual void BeginPlay() override;
	virtual void UseSkill() override;
};

