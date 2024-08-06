#pragma once

#include "Event.h"

#include "HitResult.h"

struct CollisionEvent : public Event<CollisionEvent> {
	HitResult hitResult;
};