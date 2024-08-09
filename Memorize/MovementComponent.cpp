#include "MovementComponent.h"
#include "../D2DGameEngine/Actor.h"

void MovementComponent::Update(float _dt)
{
	parent->Translate(direction.x * speed * _dt, direction.y * speed * _dt);
}