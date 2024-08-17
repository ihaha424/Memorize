#include "RazerCircle.h"

RazerCircle::RazerCircle(World* _world) : BossSkillActor(_world)
{
	isDispel = true;
	isFragile = false;
	type = Projectile;
	dispelTime = 0.f;
	skillDuration = 0.f;
	damage = 0.f;
	speed = 0.f;
	duration = 0.f;





}

void RazerCircle::FixedUpdate(float _fixedRate)
{
}

void RazerCircle::Update(float _dt)
{
}
