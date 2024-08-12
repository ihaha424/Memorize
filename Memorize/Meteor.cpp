#include "Meteor.h"
#include "MovementComponent.h"
#include "D2DGameEngine/BitmapComponent.h"
#include "MeteorEffect.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Mouse.h"
#include "D2DGameEngine/Reflection.h"
#include "Player.h"

Meteor::Meteor(Actor* _owner) : RangeSkill(_owner)
{
	SetID(ST_RANGE, SE_FIRE);
	ReflectionIn();

	//메테오 효과 액터 생성
	meteorEffect = GetWorld()->GetCurLevel()->CreateActor<MeteorEffect>();
	
}

Meteor::~Meteor()
{
}


void Meteor::UseSkill()
{
	meteorEffect->Activate();
	Math::Vector2 attackPos = { Mouse::curMousePosition.x, Mouse::curMousePosition.y };
	attackPos = GetWorld()->ScreenToWorldPoint(attackPos);
	meteorEffect->SetAttackPos(attackPos);
	meteorEffect->SetOwner(player);

	meteorEffect->SetLocation(attackPos.x + initialHeight, attackPos.y - initialHeight);
	Math::Vector2 direction = attackPos - meteorEffect->GetLocation();
	direction.Normalize();

	meteorEffect->SetVelocity(direction, fallSpeed);

}

void Meteor::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/Meteor.txt");
	reflectionResource->ParsingFile(0, damage, mana, commandList, conditionCount, castingTime, bCrash, text);
}
