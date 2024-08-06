#include "Player.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "../D2DGameEngine/CameraComponent.h"
#include "../D2DGameEngine/World.h"

Player::Player(class World* _world) : Character(_world)
{
	SetTickProperties(TICK_UPDATE | TICK_RENDER);

	BitmapComponent* bm = CreateComponent<BitmapComponent>();
	rootComponent = bm;
	CameraComponent* cm = CreateComponent<CameraComponent>();
	GetWorld()->SetMainCamera(cm);
	bm->AddChild(cm);

	bm->SetSprite(L"Memorize/testPlayer.png");
}

Player::~Player()
{
}
