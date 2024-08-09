#include "Meteor.h"
#include "MovementComponent.h"
#include "D2DGameEngine/BitmapComponent.h"
#include "D2DGameEngine/Actor.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Mouse.h"

Meteor::Meteor(Actor* _owner) : RangeSkill(_owner)
{
	SetTickProperties(TICK_UPDATE);
	SetID(ST_RANGE, SE_FIRE);

	//메테오 효과 액터 생성
	meteorEffect = GetWorld()->GetCurLevel()->CreateActor<Actor>();
	meteorEffect->SetTickProperties(TICK_UPDATE | TICK_RENDER);
	BitmapComponent* bm = meteorEffect->CreateComponent<BitmapComponent>();
	meteorEffect->rootComponent = bm;
	bm->SetSprite(L"TestResource/Skill/Range/Meteor.png");

	movementComponent = meteorEffect->CreateComponent<MovementComponent>();
	bm->AddChild(movementComponent);
}

Meteor::~Meteor()
{
}

void Meteor::BeginPlay()
{
	__super::BeginPlay();

	meteorEffect->SetStatus(OS_INACTIVE);

}

void Meteor::Update(float _dt)
{
	if (meteorEffect->GetStatus() == OS_ACTIVE)
	{
		if (Math::Vector2::Distance(meteorEffect->GetLocation(), attackPos) < 10)
		{
			meteorEffect->SetStatus(OS_INACTIVE);

			//TODO
			//대미지 입히기 
		}
	}
}

void Meteor::UseSkill()
{
	meteorEffect->SetStatus(OS_ACTIVE);
	Math::Vector2 mousePos = { Mouse::curMousePosition.x, Mouse::curMousePosition.y };
	mousePos = GetWorld()->ScreenToWorldPoint(mousePos);
	attackPos = mousePos;
	meteorEffect->SetLocation(mousePos.x + initialHeight, mousePos.y - initialHeight);
	Math::Vector2 direction = attackPos - meteorEffect->GetLocation();
	direction.Normalize();
	movementComponent->SetDirection(direction);
	movementComponent->SetSpeed(fallSpeed);

}
