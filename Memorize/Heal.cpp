#include "Heal.h"
#include "D2DGameEngine/ResourceManager.h"
#include "Player.h"
#include "BuffEffectComponent.h"
#include "D2DGameEngine/AnimationState.h"

Heal::Heal(Actor* _owner) : BuffSkill(_owner)
{
	SetID(ST_SPECIAL, SE_WATER);
	ReflectionIn();
	buffStat = Stat(healHP, 0.f);
}

Heal::~Heal()
{
}

void Heal::Initialize()
{
	__super::Initialize();

	
}

void Heal::UseSkill()
{
	__super::UseSkill();

	BuffEffectComponent* abm = player->buffEffect;
	abm->SetSprite(L"TestResource/Player/Skill/Skill_Heal02.png");
	abm->SliceSpriteSheet(500, 500, 0, 0, 0, 0);
	abm->SetFrameDurations({ 1.0f / 24.f });
	abm->Trigger(false);
	abm->Trigger(true);
	abm->SetLoop(true);
	player->buffEffect->SetStatus(OS_ACTIVE);
	player->StartHeadEffect(1);

	SoundManager::PlayMusic(L"TestResource/Sound/Player/Skill/Sound_Heal.wav");
}

void Heal::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/Heal.txt");
	reflectionResource->ParsingFile(0, mana, commandList, conditionCount, castingTime, text, infoText);
}
