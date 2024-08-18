#include "DarkSphere.h"
#include "D2DGameEngine/World.h"
#include "Player.h"
#include "DarkSphereProjectile.h"
#include "D2dGameEngine/ResourceManager.h"

DarkSphere::DarkSphere(Actor* _owner) : ProjectileSkill(_owner)
{
	projectileCount = 8;
	ReflectionIn();
	SetID(ST_PROJECTILE, SE_DARKNESS);

	for (int i = 0; i < projectileCount; i++)
	{
		projectiles.push_back(GetWorld()->GetCurLevel()->CreateActor<DarkSphereProjectile>());
		projectiles[i]->SetVelocity({ 0,0 }, 0);
		projectiles[i]->SetDamage(damage);
		projectiles[i]->SetDuration(projectileDuration);
	}
	skillDuration = projectileDuration;
}

DarkSphere::~DarkSphere()
{
}

void DarkSphere::UseSkill()
{
	__super::UseSkill();
	for (int i = 0; i < projectileCount; i++)
	{
		Projectile* nowPj = projectiles[i];
		nowPj->Initialize();
		nowPj->SetDelay(0.2f * i);
		nowPj->SetLocation(player->GetLocation().x, player->GetLocation().y);
		nowPj->SetVelocity(attackDir, projectileSpeed);
		nowPj->Activate();
	}

}

void DarkSphere::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/DarkSphere.txt");
	reflectionResource->ParsingFile(0, damage, mana, commandList, projectileSpeed,
		projectileDuration, conditionCount, castingTime, bCrash, text);
}
