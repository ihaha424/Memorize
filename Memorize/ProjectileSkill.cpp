#include "ProjectileSkill.h"
#include "Projectile.h"
#include "D2DGameEngine/Mouse.h"
#include "D2DGameEngine/World.h"
#include "Player.h"

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
			pj->Destroy();
		}
	}
}

void ProjectileSkill::UseSkill()
{
	__super::UseSkill();

	//시전 방향 
	Math::Vector2 mousePos = { Mouse::curMousePosition.x, Mouse::curMousePosition.y };
	mousePos = GetWorld()->ScreenToWorldPoint(mousePos);
	attackDir = mousePos - Math::Vector2(player->GetLocation().x, player->GetLocation().y);
	attackDir.Normalize();
	bEnd = false;
	projectiles[nowUsingCount]->Initialize();
	projectiles[nowUsingCount]->SetSkillID(id);
	projectiles[nowUsingCount]->SetOwnerSkill(this);
}
