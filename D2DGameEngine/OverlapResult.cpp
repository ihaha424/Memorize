#include "OverlapResult.h"

#include "Actor.h"
#include "PrimitiveComponent.h"

Actor* OverlapResult::GetActor()
{
	return component->GetOwner();
}

PrimitiveComponent* OverlapResult::GetComponent()
{
	return component;
}
