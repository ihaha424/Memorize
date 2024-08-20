#include "Fireball.h"
#include "FireballProjectile.h"
#include "Player.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "../D2DGameEngine/CameraComponent.h"
#include "../D2DGameEngine/Mouse.h"
#include "../D2DGameEngine/World.h"
#include "D2dGameEngine/ResourceManager.h"


Fireball::Fireball(Actor* _owner) : ProjectileSkill(_owner)
{
	SetID(ST_PROJECTILE, SE_FIRE);
	ReflectionIn();
	projectileCount = 2;

	for (int i = 0; i < projectileCount; i++)
	{
		projectiles.push_back(GetWorld()->GetCurLevel()->CreateActor<FireballProjectile>());
		projectiles[i]->SetVelocity({ 0,0 }, 0);
		projectiles[i]->SetDamage(damage);
		projectiles[i]->SetSpeed(projectileSpeed); 
		projectiles[i]->SetDuration(projectileDuration);
	}
	skillDuration = projectileDuration;
}

Fireball::~Fireball()
{
}


void Fireball::UseSkill()
{
	__super::UseSkill();
	//���̾ ù ��ġ ����
	Projectile* fireball = projectiles[nowUsingCount];
	fireball->SetPlayer(player);
	fireball->SetLocation(player->GetLocation().x, player->GetLocation().y);
	fireball->SetStatus(OS_ACTIVE);

	//���콺 ��ġ�� �̵���Ŵ
	fireball->SetVelocity(attackDir, projectileSpeed);
	nowUsingCount = (nowUsingCount + 1 ) % projectileCount;
}

void Fireball::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/Fireball.txt");
	reflectionResource->ParsingFile(0, damage, mana, commandList, projectileSpeed,
		projectileDuration, conditionCount, castingTime, bCrash, text);
}

