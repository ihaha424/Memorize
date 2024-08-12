#include "MovementComponent.h"
#include "../D2DGameEngine/Actor.h"

void MovementComponent::Update(float _dt)
{
	parent->AddComponentVelocity({ direction.x * speed * _dt, direction.y * speed * _dt });
}