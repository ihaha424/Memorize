#include "MPRecovery.h"
#include "Player.h"
#include "D2DGameEngine/ResourceManager.h"
#include "BuffEffectComponent.h"
#include "D2DGameEngine/AnimationState.h"

MPRecovery::MPRecovery(Actor* _owner) : BuffSkill(_owner)
{
	SetID(ST_BUFF, SE_DARKNESS);
	ReflectionIn();

	buffStat =
	{
		0.f,
		0.f,
		0.f,
		0.f,
		0.f,
		mpRecovery,
		0.f,
		0,
		0,
		0
	};
}

MPRecovery::~MPRecovery()
{
}

void MPRecovery::Initialize()
{
	__super::Initialize();

	animState = player->buffEffect->CreateState<AnimationState>();
	animState->SetSprite(L"TestResource/Player/Skill/Skill_MPrecovery.png");
	animState->SliceSpriteSheet(250, 330, 0, 0, 0, 0);
	animState->FrameResize(20);
	animState->SetFrameDurations({ castingTime/ 20.f });
	player->buffEffect->Initialize(animState);
}

void MPRecovery::UseSkill()
{
	__super::UseSkill();

	player->buffEffect->SetState(animState);
	player->buffEffect->SetStatus(OS_ACTIVE);
}

void MPRecovery::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/MPrecovery.txt");
	reflectionResource->ParsingFile(0, mana, commandList, conditionCount, castingTime, text);
}
