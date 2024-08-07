#pragma once
#include "Skill.h"
class ProjectileSkill : public Skill
{
protected:
	std::vector<class Projectile*> projectiles;

	int projectileCount = 1;

public:
	ProjectileSkill(class World* _world, std::wstring _id);
	virtual ~ProjectileSkill();


};

