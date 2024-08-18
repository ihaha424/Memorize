#pragma once
#include "RangeSkill.h"
class ElementalExplosion : public RangeSkill
{
	class ElementalExplosionEffect* elementalExplosionEffect;
public:
	ElementalExplosion(Actor* _owner);
	virtual ~ElementalExplosion();

	virtual void UseSkill() override;
	virtual void ReflectionIn() override;
};

