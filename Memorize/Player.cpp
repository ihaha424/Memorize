#include "Player.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "../D2DGameEngine/CameraComponent.h"

Player::Player()
{
	BitmapComponent* bm = CreateDefaultSubobject<BitmapComponent>();
	bm->SetOwner(this);
	rootComponent = bm;
	CameraComponent* cm = CreateDefaultSubobject<CameraComponent>();
	bm->AddChild(cm);

	bm->Translate({ 100, 100 });
	bm->SetSprite(L"testPlayer.png");
}

Player::~Player()
{
}
