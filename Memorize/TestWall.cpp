#include "TestWall.h"

#include "D2DGameEngine/BoxComponent.h"
#include "D2DGameEngine/BitmapComponent.h"

TestWall::TestWall(class World* _world) : Actor(_world)
{
	SetTickProperties(TICK_RENDER);

	BoxComponent* box = CreateComponent<BoxComponent>();
	box->collisionProperty = CollisionProperty(CollsionPropertyPreset::BlockAll);
	box->bGenerateOverlapEvent = true;
	rootComponent = box;

	BitmapComponent* bitmap = CreateComponent<BitmapComponent>();
	bitmap->SetSprite(L"TestResource/Player/PlayerMotions/Fire.png");

	box->InitBoxExtent({ bitmap->GetSpriteWidth(), bitmap->GetSpriteHeight() });

	rootComponent->AddChild(bitmap);
}
