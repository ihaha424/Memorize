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
	projectileMaxCount = 3;
}

ChasingWaterBall::~ChasingWaterBall()
{
}

void ChasingWaterBall::BeginPlay()
{
	for (int i = 0; i < projectileMaxCount; i++)
	{
		projectiles.push_back(GetWorld()->GetCurLevel()->CreateActor<ChasingWaterBallProjectile>());
		projectiles[i]->Inactivate();
	}
}


void ChasingWaterBall::UseSkill()
{
	////���̾ ù ��ġ ����
	//Projectile* fireball = projectiles[nowUsingCount];
	//fireball->SetLocation(player->GetLocation().x, player->GetLocation().y);

	////���콺 ��ġ�� �̵���Ŵ
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
