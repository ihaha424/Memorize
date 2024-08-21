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
	meteorEffect->SetSkillID(id);
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
	meteorEffect->SetPlayer(player);

	meteorEffect->SetLocation(player->GetLocation().x, attackPos.y - initialHeight);
	Math::Vector2 direction = attackPos - meteorEffect->GetLocation();
	direction.Normalize();

	meteorEffect->SetVelocity(direction, fallSpeed);
	meteorEffect->SetDamage(damage);
	meteorEffect->Initialize();

	//방향에 맞게 회전
	double rotateRad = std::acos(direction.Dot(Math::Vector2(1.f, 0.f)));
	if (direction.y < 0)
		rotateRad *= -1;
	meteorEffect->rootComponent->SetRotation(rotateRad * 180.f / PI - 90);

}

void Meteor::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/Meteor.txt");
	reflectionResource->ParsingFile(0, damage, mana, commandList, conditionCount, castingTime, bCrash, text);
}
