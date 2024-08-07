#include "Fireball.h"
#include "Projectile.h"
#include "Player.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "../D2DGameEngine/CameraComponent.h"
#include "../D2DGameEngine/Mouse.h"
#include "../D2DGameEngine/World.h"


Fireball::Fireball(World* _world) : ProjectileSkill(_world, "S11000")
{
}

Fireball::~Fireball()
{
}

void Fireball::UseSkill()
{
	Projectile* fireball = new Projectile(world);

	BitmapComponent* bm = fireball->GetComponent<BitmapComponent>();
	bm->SetSprite(L"Resources/fireball.png");

	fireball->SetLocation(player->GetLocation().x, player->GetLocation().y);

	Math::Vector2 mousePos = { Mouse::curMousePosition.x, Mouse::curMousePosition.y };
	mousePos = GetWorld()->ScreenToWorldPoint(mousePos);
	fireball->SetVelocity(mousePos, speed);
}

