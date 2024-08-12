#include "Character.h"

#include "BoxComponent.h"

Character::Character(class World* _world) : Pawn(_world)
{
	// NOTE: Test collision
	BoxComponent* box = CreateComponent<BoxComponent>();
	box->collisionProperty = CollisionProperty(CollsionPropertyPreset::BlockAll);
	box->bSimulatePhysics = true;
	box->bApplyImpulseOnDamage = false;
	box->bGenerateOverlapEvent = true;
	box->InitBoxExtent({ 124, 220 });
	rootComponent = box;
}

Character::~Character()
{
}
