#include "PrismReflection.h"
#include "PrismReflectionProjectile.h"
#include "../D2DGameEngine/World.h"
#include "D2DGameEngine/Mouse.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/RandomGenerator.h"
#include "Player.h"

PrismReflection::PrismReflection(Actor* _owner) : ProjectileSkill(_owner)
{
	projectileMaxCount = 9;
}

PrismReflection::~PrismReflection()
{
}

void PrismReflection::BeginPlay()
{
	__super::BeginPlay();
	for (int i = 0; i < projectileMaxCount; i++)
	{
		projectiles.push_back(GetWorld()->GetCurLevel()->CreateActor<PrismReflectionProjectile>());
		projectiles[i]->SetVelocity({ 0,0 }, 0);
		projectiles[i]->SetStatus(OS_INACTIVE);
	}
}

void PrismReflection::UseSkill()
{
	__super::UseSkill();

	//시전 방향 
	Math::Vector2 mousePos = { Mouse::curMousePosition.x, Mouse::curMousePosition.y };
	mousePos = GetWorld()->ScreenToWorldPoint(mousePos);
	attackDir = mousePos - Math::Vector2(player->GetLocation().x, player->GetLocation().y);
	attackDir.Normalize();

	//시전 방향 기준 5도 간격 9방향 체크 
	std::vector<Math::Vector2> directions;
	for (int i = -4; i <= 4; ++i)
	{
		float radians = i * 5 * PI / 180.f;
		Math::Vector2 newDir =
		{
			attackDir.x * std::cos(radians) - attackDir.y * std::sin(radians),
			attackDir.x * std::sin(radians) + attackDir.y * std::cos(radians)
		};
		directions.push_back(newDir);
	}
	
	//랜덤으로 N 방향에 투사체 발사 
	for (int i = 0; i < projectileCount; i++)
	{
		int n = Random::Get<int>(9);
	}
}
