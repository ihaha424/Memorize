#include "BossChasingProjectile.h"

#include "D2DGameEngine/World.h"
#include "D2DGameEngine/ReflectionResource.h"
#include "D2DGameEngine/ResourceManager.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "D2DGameEngine/PolygonComponent.h"

#include "Player.h"

ChasingProjectile::ChasingProjectile(World* _world) : BossSkillActor(_world)
{
	bm->isVisible = false;

	isDispel = false;
	isFragile = true;
	type = Projectile;
	dispelTime = 0.f;
	skillDuration = 3.f;
	// std::max(DispelTime - SkillDuration, 0);
	damage = 20.f;
	speed = 400.f;
	duration = 4.f;




}

void ChasingProjectile::BeginPlay()
{
}

void ChasingProjectile::FixedUpdate(float _fixedRate)
{
}

void ChasingProjectile::Update(float _dt)
{
}

void ChasingProjectile::DisfellAction()
{
}

void ChasingProjectile::OnBeginOverlap(Actor* other, const OverlapInfo& overlap)
{
}

void ChasingProjectile::OnEndOverlap(Actor* other, const OverlapInfo& overlap)
{
}

void ChasingProjectile::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PatternData/Pattern05_Actor.txt");
	int typeInt;
	reflectionResource->ParsingFile(0, isFragile, typeInt, dispelTime, skillDuration, damage);
	type = static_cast<BossSkillType>(typeInt);
}

void ChasingProjectile::ReflectionOut()
{
}
