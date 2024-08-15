#include "MagicCircle.h"
#include "D2DGameEngine/Mouse.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/BitmapComponent.h"

MagicCircle::MagicCircle(Actor* _owner) : SceneComponent(_owner)
{
	SetTickProperties(TICK_UPDATE);
	bm0 = _owner->CreateComponent<BitmapComponent>();
	bm0->SetSprite(L"TestResource/Player/MagicCircle/Fire/Magic_Circle_F1.png");
	AddChild(bm0);
}

MagicCircle::~MagicCircle()
{
}

void MagicCircle::Update(float _dt)
{
	__super::Update(_dt);

	Math::Vector2 mousePos = { Mouse::curMousePosition.x, Mouse::curMousePosition.y };
	mousePos = GetWorld()->ScreenToWorldPoint(mousePos);
	Math::Vector2 direction = mousePos - Math::Vector2(GetOwner()->GetLocation().x, GetOwner()->GetLocation().y);
	direction.Normalize();

	SetTranslation(direction.x * distance, direction.y * distance);

	bm0->SetScale(0.75 * abs(direction.Dot(Math::Vector2(0, 1))) + 0.25, 1);
}
