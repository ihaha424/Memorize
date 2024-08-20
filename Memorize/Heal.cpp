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
	abm->SetSprite(L"TestResource/Player/Skill/Skill_Enchant.png");
	abm->SliceSpriteSheet(250, 330, 0, 0, 0, 0);
	abm->FrameResize(20);
	abm->SetFrameDurations({ castingTime / 20.f });
	abm->Trigger(true);
	abm->SetLoop(true);
	player->buffEffect->SetStatus(OS_ACTIVE);
}

void Heal::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/Heal.txt");
	reflectionResource->ParsingFile(0, mana, commandList, conditionCount, castingTime, text);
}
