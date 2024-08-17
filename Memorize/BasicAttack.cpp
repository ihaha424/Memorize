#include "BasicAttack.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Level.h"
#include "BaseAttackProjectile.h"
#include "Player.h"

BasicAttack::BasicAttack(Actor* _owner) : ProjectileSkill(_owner)
{
	SetID(ST_NONE, SE_NONE);
	projectileCount = 5;

	for (int i = 0; i < projectileCount; i++)
	{
		projectiles.push_back(GetWorld()->GetCurLevel()->CreateActor<BaseAttackProjectile>());
		projectiles[i]->SetDuration(projectileDuration); 
		projectiles[i]->SetSpeed(projectileSpeed);
	}

}

BasicAttack::~BasicAttack()
{
}

void BasicAttack::UseSkill()
{
	__super::UseSkill();

	projectiles[nowUsingCount]->Activate();

	projectiles[nowUsingCount]->SetLocation(player->GetLocation().x, player->GetLocation().y);
	projectiles[nowUsingCount]->SetVelocity(attackDir, projectileSpeed);

	nowUsingCount = (nowUsingCount + 1) % projectileCount;
}
