#include "LeftRock.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"

LeftRock::LeftRock(World* _world) : Actor(_world)
{
	AnimationBitmapComponent* abm = CreateComponent<AnimationBitmapComponent>();
	abm->SetSprite(L"TestResource/Main/04.png");
	abm->SliceSpriteSheet(322, 473, 0, 0, 0, 0);
	rootComponent = abm;

	rootComponent->SetTranslation(-1920 / 2 + 35 + 322 / 2, -150 - 35);
	tween = new DotTween<float>;
	tween->SetData(&value);
}

LeftRock::~LeftRock()
{
}

void LeftRock::Update(float _dt)
{
}
