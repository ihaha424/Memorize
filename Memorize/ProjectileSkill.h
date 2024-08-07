#pragma once
#include "Skill.h"
class ProjectileSkill : public Skill
{
protected:
	std::vector<class Projectile*> projectiles;

	int projectileCount = 1;
	int projectileMaxCount = 1;
	int nowUsingCount = 0;
	float projectileSpeed = 10.f;

public:
	ProjectileSkill(class World* _world, std::wstring _id);
	virtual ~ProjectileSkill();

};

