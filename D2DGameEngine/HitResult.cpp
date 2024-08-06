#include "HitResult.h"

#include "PrimitiveComponent.h"
#include "Actor.h"

Actor* HitResult::GetActor()
{
	return hitComponent->GetOwner();
}
