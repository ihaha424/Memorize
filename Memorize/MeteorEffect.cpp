#include "MeteorEffect.h"
#include "D2DGameEngine/BitmapComponent.h"
#include "MovementComponent.h"

MeteorEffect::MeteorEffect(World* _world) : Actor(_world)
{
	SetTickProperties(TICK_UPDATE | TICK_RENDER);
	bitmap = CreateComponent<BitmapComponent>();
	rootComponent = bitmap;
	bitmap->SetSprite(L"TestResource/Skill/Range/Meteor.png");

	movement = CreateComponent<MovementComponent>();
	bitmap->AddChild(movement);
}

MeteorEffect::~MeteorEffect()
{
}

void MeteorEffect::BeginPlay()
{
	Inactivate();
}

void MeteorEffect::Update(float _dt)
{
	if (GetStatus() == OS_ACTIVE)
	{
		if (Math::Vector2::Distance(GetLocation(), attackPos) < 10)
		{
			Inactivate();

			//TODO
			//대미지 입히기 
		}
	}
}

void MeteorEffect::Initialize()
{
}
