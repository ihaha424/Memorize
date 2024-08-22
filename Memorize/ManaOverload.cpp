#include "ManaOverload.h"
#include "D2DGameEngine/ResourceManager.h"
#include "GPlayerController.h"
#include "Player.h"
#include "BuffEffectComponent.h"
#include "ManaOverloadComponent.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"

ManaOverload::ManaOverload(Actor* _owner) : BuffSkill(_owner)
{
	SetID(ST_BUFF, SE_WATER);
	ReflectionIn();
}

ManaOverload::~ManaOverload()
{
}

void ManaOverload::Initialize()
{
	__super::Initialize();

	effect = player->manaOverloadEffect;
	mana = -1;
}

void ManaOverload::UseSkill()
{
	__super::UseSkill();
	controller->bManaOverload = true;

	effect->SetStatus(OS_ACTIVE);
	SoundManager::PlayMusic(L"TestResource/Sound/Player/Skill/Sound_ManaOverload.wav");
}

void ManaOverload::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/ManaOverload.txt");
	reflectionResource->ParsingFile(0, mana, commandList, conditionCount, castingTime, text);
}
