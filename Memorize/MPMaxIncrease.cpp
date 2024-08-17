#include "MPMaxIncrease.h"
#include "Player.h"
#include "D2DGameEngine/ResourceManager.h"
#include "BuffEffectComponent.h"
#include "D2DGameEngine/AnimationState.h"

MPMaxIncrease::MPMaxIncrease(Actor* _owner) : BuffSkill(_owner)
{
	SetID(ST_BUFF, SE_LIGHT);
	ReflectionIn();
	buffStat = Stat(0.f, 0.f, 0.f, additionalMP);
}

MPMaxIncrease::~MPMaxIncrease()
{
}

void MPMaxIncrease::Initialize()
{
	__super::Initialize();

	animState = player->buffEffect->CreateState<AnimationState>();
	animState->SetSprite(L"TestResource/Player/Skill/Skill_MPrecovery.png");
	animState->SliceSpriteSheet(250, 330, 0, 0, 0, 0);
	animState->FrameResize(20);
	animState->SetFrameDurations({ 0.025 });
	player->buffEffect->Initialize(animState);
}

void MPMaxIncrease::UseSkill()
{
	__super::UseSkill();

	player->buffEffect->SetState(animState);
	player->buffEffect->SetStatus(OS_ACTIVE);
}

void MPMaxIncrease::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/MPMaxIncrease.txt");
	reflectionResource->ParsingFile(0, mana, commandList, conditionCount, castingTime, text);
}
