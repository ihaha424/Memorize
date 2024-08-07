#include "ProjectileSkill.h"
#include "Projectile.h"

ProjectileSkill::ProjectileSkill()
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