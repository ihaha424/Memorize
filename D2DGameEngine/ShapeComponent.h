#pragma once

#include "PrimitiveComponent.h"

class ShapeComponent : public PrimitiveComponent {

public:
	ShapeComponent(Actor* _owner)  : PrimitiveComponent(_owner){
		isVisible = false;
		SetCollisionEnabled(CollisionEnabled::EnableCollision);
		SetTickProperties(TICK_PHYSICS | TICK_RENDER);
	}
};
