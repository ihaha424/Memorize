#include "Player.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "GCameraComponent.h"
#include "MovementComponent.h"
#include "../D2DGameEngine/World.h"
#include "../D2DGameEngine/ReflectionResource.h"
#include "../D2DGameEngine/ResourceManager.h"

Player::Player(class World* _world) : Character(_world)
{
	ReflectionIn();

	SetTickProperties(TICK_UPDATE | TICK_RENDER | TICK_POST_UPDATE);

	BitmapComponent* bm = CreateComponent<BitmapComponent>();
	rootComponent = bm;
	GCameraComponent* cm = CreateComponent<GCameraComponent>();
	GetWorld()->SetMainCamera(cm);
	bm->AddChild(cm);
	bm->SetSprite(L"Memorize/testPlayer.png");

	MovementComponent* mv = CreateComponent< MovementComponent>();
}

Player::~Player()
{
}

void Player::LevelUp()
{
	if (level < maxLevel)
	{
		level++;
		exp = 0;
	}

	//TODO
	//level에 따라 Stat 조정
}

void Player::AddToStat(Stat _addStat)
{
	stat = stat + _addStat;
}

void Player::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Player.txt");
	reflectionResource->ParsingFile(0, moveSpeed, ReflectionData, ReflectionData1, ReflectionData2, ReflectionData3);
}

void Player::ReflectionOut()
{}
