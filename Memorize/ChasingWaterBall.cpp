#include "ChasingWaterBall.h"
#include "ChasingWaterBallProjectile.h"
#include "Player.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "../D2DGameEngine/CameraComponent.h"
#include "../D2DGameEngine/Mouse.h"
#include "../D2DGameEngine/World.h"
#include "../D2DGameEngine/CircleComponent.h"


ChasingWaterBall::ChasingWaterBall(Actor* _owner) : ProjectileSkill(_owner)
{
	SetTickProperties(TICK_UPDATE);
	ReflectionIn();

	SetID(ST_PROJECTILE, SE_WATER);

	projectileCount = 5;

	locations.resize(projectileCount);
	locations[0] = {-120,-80};
	locations[1] = {-60,-140};
	locations[2] = {0,-180};
	locations[3] = {60,-140};
	locations[4] = {120,-80};

	
	
	skillDuration = projectileDuration;

}

ChasingWaterBall::~ChasingWaterBall()
{
}


void ChasingWaterBall::UseSkill()
{
	__super::UseSkill();

	projectiles.clear();
	for (int i = 0; i < projectileCount; i++)
	{
		projectiles.push_back(GetWorld()->GetCurLevel()->CreateActor<ChasingWaterBallProjectile>());
		projectiles[i]->SetDamage(damage);
		projectiles[i]->SetDuration(projectileDuration);
		projectiles[i]->SetSpeed(projectileSpeed);
		ChasingWaterBallProjectile* waterballpj = static_cast<ChasingWaterBallProjectile*>(projectiles[i]);
		waterballpj->SetInitialLocation(locations[i].x, locations[i].y);

	}

	for (int i = 0; i < projectileCount; i++)
	{
		projectiles[i]->SetPlayer(player);
		projectiles[i]->Activate();
		projectiles[i]->SetVelocity({ 0.f, 0.f }, 0.f);
		projectiles[i]->Initialize();
		projectiles[i]->SetSkillID(id);
	}
	
}

void ChasingWaterBall::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/ChasingWaterBall.txt");
	reflectionResource->ParsingFile(0, damage, mana, commandList, projectileSpeed,
		projectileDuration, conditionCount, castingTime, bCrash, text);
}

