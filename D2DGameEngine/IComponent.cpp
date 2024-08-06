#pragma once

#include "Actor.h"

World* IComponent::GetWorld() const
{
	return owner->GetWorld();
}
