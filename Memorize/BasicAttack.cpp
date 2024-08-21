#include "BasicAttack.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Level.h"
#include "BaseAttackProjectile.h"
#include "Player.h"

BasicAttack::BasicAttack(Actor* _owner) : ProjectileSkill(_owner)
{
	SetID(ST_NONE, SE_NONE);
	projectileCount = 5;
	projectileSpeed = 800.f;
	projectileDuration = 1.f;
	for (int i = 0; i < projectileCount; i++)
	{
		projectiles.push_back(GetWorld()->GetCurLevel()->CreateActor<BaseAttackProjectile>());
		projectiles[i]->SetDuration(projectileDuration);
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
	projectiles[nowUsingCount]->SetDamage(player->stat.defaultDamage);

	//방향에 맞게 회전
	double rotateRad = std::acos(attackDir.Dot(Math::Vector2(1.f, 0.f)));
	if (attackDir.y < 0)
		rotateRad *= -1;
	projectiles[nowUsingCount]->rootComponent->SetRotation(rotateRad * 180.f / PI + 180);

	nowUsingCount = (nowUsingCount + 1) % projectileCount;
}
