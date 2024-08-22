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

}

void MPMaxIncrease::UseSkill()
{
	__super::UseSkill();

	BuffEffectComponent* abm = player->buffEffect;
	abm->SetSprite(L"TestResource/Player/Skill/Skill_MPMaxIncrease.png");
	abm->SliceSpriteSheet(250, 350, 0, 0, 0, 0);
	abm->FrameResize(17);
	abm->SetFrameDurations({ castingTime / 17 });
	abm->Trigger(false); 
	abm->Trigger(true);
	abm->SetLoop(true);
	player->buffEffect->SetStatus(OS_ACTIVE);

	SoundManager::PlayMusic(L"TestResource/Sound/Player/Skill/Sound_MPMaxIncrease.wav");
}

void MPMaxIncrease::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/MPMaxIncrease.txt");
	reflectionResource->ParsingFile(0, mana, commandList, conditionCount, castingTime, text, infoText);
}
