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
	elementalExplosionEffect->Activate();
	Math::Vector2 pos = player->GetLocation();
	elementalExplosionEffect->SetLocation(pos.x, pos.y);

	BuffEffectComponent* abm = player->buffEffect;
	abm->SetSprite(L"TestResource/Player/Skill/Skill_ElementalExplosion1.png");
	abm->SliceSpriteSheet(300, 300, 0, 0, 0, 0);
	abm->SetFrameDurations({ 2.0f / 48.f });
	abm->Trigger(false);
	abm->Trigger(true);
	abm->SetLoop(true);
	abm->SetStatus(OS_ACTIVE);
	player->StartHeadEffect(2);
}

void ElementalExplosion::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/ElementalExplosion.txt");
	reflectionResource->ParsingFile(0, mana, commandList, conditionCount, castingTime, bCrash, text);
}
