#include "DarkSphere.h"
#include "D2DGameEngine/World.h"
#include "DarkSphereSpawner.h"
#include "Player.h"

DarkSphere::DarkSphere(Actor* _owner) : ProjectileSkill(_owner)
{
	projectileMaxCount = 8;
	SetID(ST_PROJECTILE, SE_DARKNESS);

	spawner = GetWorld()->GetCurLevel()->CreateActor<DarkSphereSpawner>();
	spawner->SetPlayer(player);
	spawner->SetPlayerController(controller);
}

DarkSphere::~DarkSphere()
{
}

void DarkSphere::UseSkill()
{
	__super::UseSkill();

	spawner->Excute(attackDir);
}
