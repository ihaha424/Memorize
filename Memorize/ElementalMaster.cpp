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
	abm = player->buffEffect;
	abm->SetSprite(L"TestResource/Player/Skill/Skill_ElementalMaster.png");
	abm->SliceSpriteSheet(550, 550, 0, 0, 0, 0);
	abm->FrameResize(97);
	abm->SetFrameDurations({ castingTime / 97.f });
	abm->Trigger(true);
	abm->SetLoop(true);
	controller->elementalMasterComponent->SetStatus(OS_ACTIVE);
	controller->bElementalMaster = true;
	player->buffEffect->SetStatus(OS_ACTIVE);
}

void ElementalMaster::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/ElementalMaster.txt");
	reflectionResource->ParsingFile(0, mana, commandList, conditionCount, castingTime, text);
}

