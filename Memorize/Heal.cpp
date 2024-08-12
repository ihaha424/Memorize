#include "Heal.h"
#include "D2DGameEngine/ResourceManager.h"

Heal::Heal(Actor* _owner) : Skill(_owner)
{
	SetID(ST_BUFF, SE_WATER);
}

Heal::~Heal()
{
}

void Heal::UseSkill()
{
	__super::UseSkill();

	player->AddToStat(Stat(healHP, 0,0));
}

void Heal::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/Heal.txt");
	reflectionResource->ParsingFile(0, mana, commandList, conditionCount, castingTime, healHP, text);

}
