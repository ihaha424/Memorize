#include "Meteor.h"
#include "MovementComponent.h"
#include "D2DGameEngine/BitmapComponent.h"
#include "D2DGameEngine/Actor.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Mouse.h"

Meteor::Meteor(Actor* _owner) : RangeSkill(_owner)
{
	SetTickProperties(TICK_UPDATE);
	//메테오 효과 컴포넌트 - 루트 씬에 연결
	metorEffectComponent = GetOwner()->CreateComponent<BitmapComponent>();
	metorEffectComponent->SetSprite(L"TestResource/Skill/Range/Meteor.png");

	GetOwner()->rootComponent->AddChild(metorEffectComponent);
	movementComponent = GetOwner()->CreateComponent<MovementComponent>();
	metorEffectComponent->AddChild(movementComponent);
}

Meteor::~Meteor()
{
}

void Meteor::BeginPlay()
{
	__super::BeginPlay();

	metorEffectComponent->SetStatus(OS_INACTIVE);

}

void Meteor::Update(float _dt)
{
	if (metorEffectComponent->GetStatus() == OS_ACTIVE)
	{
		Math::Vector2 effectPos = { metorEffectComponent->GetWorldTransform().dx, metorEffectComponent->GetWorldTransform().dy };
		if (Math::Vector2::Distance(effectPos, attackPos) < 0.1)
		{
			metorEffectComponent->SetStatus(OS_INACTIVE);

			//TODO
			//대미지 입히기 
		}
	}
}

void Meteor::UseSkill()
{
	metorEffectComponent->SetStatus(OS_ACTIVE);
	Math::Vector2 mousePos = { Mouse::curMousePosition.x, Mouse::curMousePosition.y };
	mousePos = GetWorld()->ScreenToWorldPoint(mousePos);
	attackPos = mousePos;
	metorEffectComponent->SetTranslation(mousePos.x + initialHeight, mousePos.y - initialHeight);
	Math::Vector2 direction = mousePos - Math::Vector2(metorEffectComponent->GetWorldTransform().dx, metorEffectComponent->GetWorldTransform().dy);
	direction.Normalize();
	movementComponent->SetDirection(direction);
	movementComponent->SetSpeed(fallSpeed);

}
