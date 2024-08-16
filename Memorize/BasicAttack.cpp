#include "BasicAttack.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Level.h"
#include "D2DGameEngine/BitmapComponent.h"
#include "Projectile.h"
#include "Player.h"

BasicAttack::BasicAttack(Actor* _owner) : ProjectileSkill(_owner)
{
	SetID(ST_NONE, SE_NONE);
	projectileCount = 5;

	for (int i = 0; i < projectileCount; i++)
	{
		projectiles.push_back(GetWorld()->GetCurLevel()->CreateActor<Projectile>());
		projectiles[i]->Inactivate();
		projectiles[i]->GetComponent<BitmapComponent>()->SetSprite(L"TestResource/Skill/Projectile/BasicAttack.png");
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
