#include "MagicBinding.h"

#include "D2DGameEngine/World.h"
#include "D2DGameEngine/ReflectionResource.h"
#include "D2DGameEngine/ResourceManager.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "D2DGameEngine/PolygonComponent.h"

MagicBinding::MagicBinding(World* _world) : BossSkillActor(_world)
{
	bm->isVisible = false;

	isDispel = true;
	isFragile = true;
	type = Range;
	dispelTime = 3.f;
	skillDuration = 3.f;

	binding = CreateComponent<AnimationBitmapComponent>();
	binding->SetSprite(L"");

}

void MagicBinding::BeginPlay()
{
}

void MagicBinding::FixedUpdate(float _fixedRate)
{
}

void MagicBinding::Update(float _dt)
{
}

void MagicBinding::DisfellAction()
{
}

void MagicBinding::ReflectionIn()
{
}

void MagicBinding::ReflectionOut()
{
}
