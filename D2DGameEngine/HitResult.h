#pragma once

#include "framework.h"

#include "PrimitiveComponent.h"

struct HitResult {
	bool isBlocking : 1;	// 

	PrimitiveComponent* hitComponent;
	
	float distance;		// The distance from the start to the hit point
	DXVec2 hitNormal;
	DXVec2 hitPoint;

	DXVec2 normal;
	float penetrationDepth;

	DXVec2 traceStart; 
	DXVec2 traceEnd;
};