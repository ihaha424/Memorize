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
	//���̾ ù ��ġ ����
	Projectile* fireball = projectiles[nowUsingCount];
	fireball->SetOwner(player);
	fireball->SetLocation(player->GetLocation().x, player->GetLocation().y);
	fireball->SetStatus(OS_ACTIVE);

	//���콺 ��ġ�� �̵���Ŵ
	fireball->SetVelocity(attackDir, projectileSpeed);
	fireball->Activate();
	nowUsingCount++;
}

