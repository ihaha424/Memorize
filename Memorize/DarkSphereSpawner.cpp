#include "DarkSphereSpawner.h"
#include "DarkSphereProjectile.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Actor.h"
#include "Player.h"
#include "GPlayerController.h"

DarkSphereSpawner::DarkSphereSpawner(World* _world) : Actor(_world)
{
	for (int i = 0; i < projectileCount; i++)
	{
		projectiles.push_back(GetWorld()->GetCurLevel()->CreateActor<DarkSphereProjectile>());
		projectiles[i]->SetVelocity({ 0,0 }, 0);
	}
	Inactivate();
}

DarkSphereSpawner::~DarkSphereSpawner()
{
}

void DarkSphereSpawner::Update(float _dt)
{
	__super::Update(_dt);

	elapsedTime += _dt;

	if (elapsedTime > period)
	{
		elapsedTime -= period;

		Projectile* darkSphere = projectiles[nowUsingCount];

		darkSphere->SetLocation(player->GetLocation().x, player->GetLocation().y);
		darkSphere->SetVelocity(direction, speed);
		nowUsingCount++;
	}

	if (nowUsingCount > projectileCount)
	{
		Inactivate();
		playerController->EndSkill();
	}

}

void DarkSphereSpawner::Excute(Math::Vector2 _direction)
{
	Activate();
	elapsedTime = 0.f;
	direction = _direction;
	Projectile* darkSphere = projectiles[nowUsingCount++];

	darkSphere->SetLocation(player->GetLocation().x, player->GetLocation().y);
	darkSphere->SetVelocity(direction, speed);
}
