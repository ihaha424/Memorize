#include "ManaOverload.h"
#include "D2DGameEngine/ResourceManager.h"
#include "GPlayerController.h"
#include "Player.h"
#include "BuffEffectComponent.h"
#include "D2DGameEngine/AnimationState.h"

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

	animState = player->buffEffect->CreateState<AnimationState>();
	animState->SetSprite(L"TestResource/Player/Skill/Skill_MPrecovery.png");
	animState->SliceSpriteSheet(250, 330, 0, 0, 0, 0);
	animState->FrameResize(20);
	animState->SetFrameDurations({ 0.025 });
	player->buffEffect->Initialize(animState);
}

void ManaOverload::UseSkill()
{
	__super::UseSkill();
	controller->bManaOverload = true;

	player->buffEffect->SetState(animState);
	player->buffEffect->SetStatus(OS_ACTIVE);
}

void ManaOverload::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/ManaOverload.txt");
	reflectionResource->ParsingFile(0, mana, commandList, conditionCount, castingTime, text);
}
