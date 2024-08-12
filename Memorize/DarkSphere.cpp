#include "DarkSphere.h"
#include "D2DGameEngine/World.h"
#include "DarkSphereSpawner.h"
#include "Player.h"
#include "D2dGameEngine/ResourceManager.h"

DarkSphere::DarkSphere(Actor* _owner) : ProjectileSkill(_owner)
{
	projectileMaxCount = 8;
	SetID(ST_PROJECTILE, SE_DARKNESS);
	commandList.push_back(0);
	commandList.push_back(3);
	commandList.push_back(3);
	commandList.push_back(3);

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
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/DarkSphere.txt");
	reflectionResource->ParsingFile(0, strId, conditionCount, mana, castingTime, commandList);
}
