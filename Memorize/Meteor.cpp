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
	meteorEffect[0] = GetWorld()->GetCurLevel()->CreateActor<MeteorEffect>();
	meteorEffect[1] = GetWorld()->GetCurLevel()->CreateActor<MeteorEffect>();
	meteorEffect[0]->SetSkillID(id);
	meteorEffect[1]->SetSkillID(id);
	castingTime = 2.5;
}

Meteor::~Meteor()
{
}


void Meteor::UseSkill()
{
	__super::UseSkill();

	meteorEffect[nowUsingIndex]->Activate();
 	Math::Vector2 attackPos = { Mouse::curMousePosition.x, Mouse::curMousePosition.y };
	attackPos = GetWorld()->ScreenToWorldPoint(attackPos);
	meteorEffect[nowUsingIndex]->SetAttackPos(attackPos);
	meteorEffect[nowUsingIndex]->SetPlayer(player);

	meteorEffect[nowUsingIndex]->SetLocation(player->GetLocation().x, attackPos.y - initialHeight);
	Math::Vector2 direction = attackPos - meteorEffect[nowUsingIndex]->GetLocation();
	direction.Normalize();

	meteorEffect[nowUsingIndex]->SetVelocity(direction, fallSpeed);
	meteorEffect[nowUsingIndex]->SetDamage(damage);
	meteorEffect[nowUsingIndex]->Initialize();

	//방향에 맞게 회전
	double rotateRad = std::acos(direction.Dot(Math::Vector2(1.f, 0.f)));
	if (direction.y < 0)
		rotateRad *= -1;
	meteorEffect[nowUsingIndex]->rootComponent->SetRotation(rotateRad * 180.f / PI - 90);

	nowUsingIndex = (nowUsingIndex + 1) % 2;
}

void Meteor::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/Meteor.txt");
	reflectionResource->ParsingFile(0, damage, mana, commandList, conditionCount, castingTime, bCrash, text);
}
