#include "DarkSphere.h"
#include "D2DGameEngine/World.h"
#include "DarkSphereSpawner.h"
#include "Player.h"
#include "D2dGameEngine/ResourceManager.h"

DarkSphere::DarkSphere(Actor* _owner) : ProjectileSkill(_owner)
{
	projectileCount = 8;
	ReflectionIn();
	
	SetID(ST_PROJECTILE, SE_DARKNESS);

	spawner = GetWorld()->GetCurLevel()->CreateActor<DarkSphereSpawner>();
	spawner->SetPlayer(player);
	spawner->SetPlayerController(controller);
}

DarkSphere::~DarkSphere()
{
}

void DarkSphere::UseSkill()
{
	__super::UseSkill();

	spawner->Excute(attackDir);
}

void DarkSphere::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/DarkSphere.txt");
	reflectionResource->ParsingFile(0, damage, mana, commandList, projectileSpeed,
		projectileDuration, conditionCount, castingTime, bCrash, text);
}
