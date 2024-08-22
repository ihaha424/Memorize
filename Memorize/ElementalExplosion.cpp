#include "ElementalExplosion.h"
#include "ElementalExplosionEffect.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/ResourceManager.h"
#include "D2DGameEngine/BitmapComponent.h"
#include "BuffEffectComponent.h"
#include "D2DGameEngine/Mouse.h"
#include "Player.h"

ElementalExplosion::ElementalExplosion(Actor* _owner) : RangeSkill(_owner)
{
	SetID(ST_SPECIAL, SE_FIRE);
	ReflectionIn();
	elementalExplosionEffect = GetWorld()->GetCurLevel()->CreateActor<ElementalExplosionEffect>();
}

ElementalExplosion::~ElementalExplosion()
{
}

void ElementalExplosion::UseSkill()
{
	__super::UseSkill();
	elementalExplosionEffect->Activate();
	Math::Vector2 pos = player->GetLocation();
	elementalExplosionEffect->SetLocation(pos.x, pos.y);

	player->StartHeadEffect(2);
	SoundManager::PlayMusic(L"TestResource/Sound/Player/Skill/Sound_ElementalExplosion01.wav");
}

void ElementalExplosion::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/ElementalExplosion.txt");
	reflectionResource->ParsingFile(0, mana, commandList, conditionCount, castingTime, bCrash, text, infoText);
}
