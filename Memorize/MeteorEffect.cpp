#include "MeteorEffect.h"
#include "D2DGameEngine/BitmapComponent.h"
#include "MovementComponent.h"

MeteorEffect::MeteorEffect(World* _world) : Projectile(_world)
{
	rootComponent = bm = CreateComponent<BitmapComponent>();
	bm->SetSprite(L"TestResource/Skill/Range/Meteor.png");

	mv = CreateComponent<MovementComponent>();
	rootComponent->AddChild(mv);
}

MeteorEffect::~MeteorEffect()
{
}

void MeteorEffect::BeginPlay()
{
	__super::BeginPlay();
	Inactivate();
}

void MeteorEffect::Update(float _dt)
{
	__super::Update(_dt);

	if (Math::Vector2::Distance(GetLocation(), attackPos) < 10)
	{
		Inactivate();

		//TODO
		//대미지 입히기 
	}
}

void MeteorEffect::Initialize()
{
}
