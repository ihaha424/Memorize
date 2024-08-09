#pragma once
#include "Skill.h"
class ProjectileSkill : public Skill
{
protected:
	std::vector<class Projectile*> projectiles;

	int projectileCount = 1;
	int projectileMaxCount = 1;
	int nowUsingCount = 0;
	float projectileSpeed = 150.f;

	Math::Vector2 attackDir;
public:
	ProjectileSkill(Actor* _owner);
	virtual ~ProjectileSkill();

	virtual void UseSkill() override;
};

