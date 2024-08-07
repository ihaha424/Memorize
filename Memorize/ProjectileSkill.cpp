#include "ProjectileSkill.h"
#include "Projectile.h"

ProjectileSkill::ProjectileSkill(World* _world, std::wstring _id)
	:Skill(_world, ST_PROJECTILE, _id)
{
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