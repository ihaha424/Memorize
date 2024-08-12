#include "MovementComponent.h"
#include "../D2DGameEngine/Actor.h"

void MovementComponent::Update(float _dt)
{
	parent->SetComponentVelocity(direction * speed);
}