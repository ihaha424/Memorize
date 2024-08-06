#pragma once

#include "Event.h"

#

struct CollisionEvent : public Event<CollisionEvent> {
	DXVec2 normal;
	float overlapDepth;


};