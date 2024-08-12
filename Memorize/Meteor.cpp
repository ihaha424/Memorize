#include "Meteor.h"
#include "MovementComponent.h"
#include "D2DGameEngine/BitmapComponent.h"
#include "MeteorEffect.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Mouse.h"
#include "D2DGameEngine/Reflection.h"

Meteor::Meteor(Actor* _owner) : RangeSkill(_owner)
{
	SetTickProperties(TICK_UPDATE);
	SetID(ST_RANGE, SE_FIRE);
	//ReflectionIn();
	commandList.push_back(1);
	commandList.push_back(0);
	commandList.push_back(0);
	commandList.push_back(2);

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

	meteorEffect->SetLocation(attackPos.x + initialHeight, attackPos.y - initialHeight);
	Math::Vector2 direction = attackPos - meteorEffect->GetLocation();
	direction.Normalize();

	MovementComponent* movement = meteorEffect->GetComponent<MovementComponent>();
	movement->SetDirection(direction);
	movement->SetSpeed(fallSpeed);

}

void Meteor::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Meteor.txt");
	reflectionResource->ParsingFile(0, strId, conditionCount, mana, castingTime, commandList);
}
