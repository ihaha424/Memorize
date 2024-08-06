#include "Player.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "../D2DGameEngine/CameraComponent.h"

Player::Player()
{
	SetTickProperties(TICK_UPDATE | TICK_RENDER);

	BitmapComponent* bm = CreateComponent<BitmapComponent>();
	rootComponent = bm;
	CameraComponent* cm = CreateComponent<CameraComponent>();
	bm->AddChild(cm);

	bm->Translate({ 100, 100 });
	bm->SetSprite(L"Memorize/testPlayer.png");
}

Player::~Player()
{
}
