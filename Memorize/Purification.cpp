#include "Purification.h"
#include "D2DGameEngine/ResourceManager.h"
#include "Player.h"
#include "GPlayerController.h"

Purification::Purification(Actor* _owner) : Skill(_owner)
{
	SetID(ST_SPECIAL, SE_LIGHT);
	ReflectionIn();
}

Purification::~Purification()
{
}

void Purification::Initialize()
{
}

void Purification::UseSkill()
{
	__super::UseSkill();

	//controller->targetSkill->SetStatus(OS_DESTROY);

	for (auto skill : disfellSkills)
	{
		skill->SetStatus(OS_DESTROY);
	}
	disfellSkills.clear();
	player->StartHeadEffect(3);
}

void Purification::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/Purification.txt");
	reflectionResource->ParsingFile(0, mana, commandList, conditionCount, castingTime, text);
}
