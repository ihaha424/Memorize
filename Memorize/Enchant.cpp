#include "Enchant.h"
#include "Player.h"
#include "D2DGameEngine/AnimationState.h"
#include "BuffEffectComponent.h"


Enchant::Enchant(Actor* _owner) : BuffSkill(_owner)
{
	SetID(ST_BUFF, SE_FIRE);
	ReflectionIn();

	buffStat = Stat(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0, 
		additionalDamage, additionalAttackSpeed);
}

Enchant::~Enchant()
{
}

void Enchant::Initialize()
{
	__super::Initialize();
}

void Enchant::UseSkill()
{
	__super::UseSkill();

	BuffEffectComponent* abm = player->buffEffect;
	abm->SetSprite(L"TestResource/Player/Skill/Skill_Enchant.png");
	abm->SliceSpriteSheet(250, 330, 0, 0, 0, 0);
	abm->FrameResize(20);
	abm->SetFrameDurations({ 0.025 });
	abm->Trigger(false); 
	abm->Trigger(true);
	abm->SetLoop(true);
	player->buffEffect->SetStatus(OS_ACTIVE);
}

void Enchant::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/Enchant.txt");
	reflectionResource->ParsingFile(0, mana, commandList, conditionCount, castingTime, text);
}
