#include "ChasingWaterBall.h"
#include "ChasingWaterBallProjectile.h"
#include "Player.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "../D2DGameEngine/CameraComponent.h"
#include "../D2DGameEngine/Mouse.h"
#include "../D2DGameEngine/World.h"


ChasingWaterBall::ChasingWaterBall(Actor* _owner) : ProjectileSkill(_owner)
{
	SetTickProperties(TICK_UPDATE);
	ReflectionIn();

	SetID(ST_PROJECTILE, SE_WATER);

	projectileCount = 5;

	for (int i = 0; i < projectileCount; i++)
	{
		projectiles.push_back(GetWorld()->GetCurLevel()->CreateActor<ChasingWaterBallProjectile>());
	}
}

ChasingWaterBall::~ChasingWaterBall()
{
}


void ChasingWaterBall::UseSkill()
{
	////파이어볼 첫 위치 지정
	//Projectile* fireball = projectiles[nowUsingCount];
	//fireball->SetLocation(player->GetLocation().x, player->GetLocation().y);

	////마우스 위치로 이동시킴
	//Math::Vector2 mousePos = { Mouse::curMousePosition.x, Mouse::curMousePosition.y };
	//mousePos = GetWorld()->ScreenToWorldPoint(mousePos);
	//fireball->SetVelocity(mousePos, projectileSpeed);

	//nowUsingCount++;


	for (int i = nowCount; i < onceClickCount; i++)
	{
		projectiles[i]->Activate();
		projectiles[i]->SetVelocity({0.f, 0.f}, 0.f);
		projectiles[i]->SetLocation(player->GetLocation().x, player->GetLocation().y);
		projectiles[i]->Initialize();
	}
}

void ChasingWaterBall::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/ChasingWaterBall.txt");
	reflectionResource->ParsingFile(0, damage, mana, commandList, projectileSpeed,
		projectileDuration, conditionCount, castingTime, bCrash, text);
}

