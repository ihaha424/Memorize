#include "Fireball.h"
#include "FireballProjectile.h"
#include "Player.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "../D2DGameEngine/CameraComponent.h"
#include "../D2DGameEngine/Mouse.h"
#include "../D2DGameEngine/World.h"


Fireball::Fireball(Actor* _owner) : ProjectileSkill(_owner)
{
	SetTickProperties(TICK_UPDATE);
	SetID(ST_PROJECTILE, SE_FIRE);
	commandList.push_back(0);
	commandList.push_back(0);

	projectileMaxCount = 5;

	for (int i = 0; i < projectileMaxCount; i++)
	{
		projectiles.push_back(GetWorld()->GetCurLevel()->CreateActor<FireballProjectile>());
		projectiles[i]->SetVelocity({ 0,0 }, 0);
	}
}

Fireball::~Fireball()
{
}


void Fireball::UseSkill()
{
	__super::UseSkill();

	if (nowUsingCount > projectileCount) return;
	//파이어볼 첫 위치 지정
	Projectile* fireball = projectiles[nowUsingCount];
	fireball->SetOwner(player);
	fireball->SetLocation(player->GetLocation().x, player->GetLocation().y);
	fireball->SetStatus(OS_ACTIVE);

	//마우스 위치로 이동시킴
	fireball->SetVelocity(attackDir, projectileSpeed);
	fireball->Activate();
	nowUsingCount++;
}

