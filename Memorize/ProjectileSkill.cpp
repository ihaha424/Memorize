#include "ProjectileSkill.h"
#include "Projectile.h"

ProjectileSkill::ProjectileSkill(Actor* _owner) : Skill(_owner)
{
	type = ST_PROJECTILE;
}

ProjectileSkill::~ProjectileSkill()
{
	for (auto pj : projectiles)
	{
		if (pj)
		{
			delete pj;
			pj = nullptr;
		}
	}
}