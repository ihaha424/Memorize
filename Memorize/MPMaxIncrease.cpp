#include "MPMaxIncrease.h"
#include "D2DGameEngine/ResourceManager.h"

MPMaxIncrease::MPMaxIncrease(Actor* _owner) : BuffSkill(_owner)
{
	SetID(ST_BUFF, SE_LIGHT);
	buffStat = Stat(0.f, 0.f, 0.f, additionalMP);
}

MPMaxIncrease::~MPMaxIncrease()
{
}

void MPMaxIncrease::UseSkill()
{
	player->AddToStat(buffStat);
}

void MPMaxIncrease::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/MPMaxIncrease.txt");
	reflectionResource->ParsingFile(0, strId, conditionCount, mana, castingTime, commandList);
}
