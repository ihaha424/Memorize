#pragma once

#include "framework.h"

class PrimitiveComponent;
struct OverlapResult {
	bool bBlockingHit : 1;
	PrimitiveComponent* component;

	class Actor* GetActor();
	PrimitiveComponent* GetComponent();
};
