#include "ElementalMaster.h"
#include "D2DGameEngine/ResourceManager.h"
#include "Player.h"
#include "BuffEffectComponent.h"
#include "GPlayerController.h"
#include "ElementalMasterComponent.h"
#include "D2DGameEngine/AnimationState.h"

ElementalMaster::ElementalMaster(Actor* _owner) : Skill(_owner)
{
	SetID(ST_SPECIAL, SE_DARKNESS);
	ReflectionIn();
	
}

ElementalMaster::~ElementalMaster()
{
}

void ElementalMaster::Initialize()
{
}

void ElementalMaster::UseSkill()
{
	__super::UseSkill();
	controller->elementalMasterComponent->SetStatus(OS_ACTIVE);
	controller->bElementalMaster = true;
}

void ElementalMaster::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/ElementalMaster.txt");
	reflectionResource->ParsingFile(0, mana, commandList, conditionCount, castingTime, text);
}

