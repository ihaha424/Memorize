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

}

void MPRecovery::UseSkill()
{
	__super::UseSkill();

	BuffEffectComponent* abm = player->buffEffect;
	abm->SetSprite(L"TestResource/Player/Skill/Skill_MPrecovery.png");
	abm->SliceSpriteSheet(250, 330, 0, 0, 0, 0);
	abm->FrameResize(20);
	abm->SetFrameDurations({ castingTime / 20.f });
	abm->Trigger(false); 
	abm->Trigger(true);
	abm->SetLoop(true);
	player->buffEffect->SetStatus(OS_ACTIVE);

	SoundManager::PlayMusic(L"TestResource/Sound/Player/Skill/Sound_MPrecovery.wav");
}

void MPRecovery::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/MPrecovery.txt");
	reflectionResource->ParsingFile(0, mana, commandList, conditionCount, castingTime, text);
}
