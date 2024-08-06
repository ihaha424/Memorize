#include "Player.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "GCameraComponent.h"
#include "../D2DGameEngine/World.h"

Player::Player(class World* _world) : Character(_world)
{
	SetTickProperties(TICK_UPDATE | TICK_RENDER);

	BitmapComponent* bm = CreateComponent<BitmapComponent>();
	rootComponent = bm;
	GCameraComponent* cm = CreateComponent<GCameraComponent>();
	GetWorld()->SetMainCamera(cm);
	bm->AddChild(cm);

	bm->SetSprite(L"Memorize/testPlayer.png");
}

Player::~Player()
{
}
