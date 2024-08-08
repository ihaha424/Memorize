#pragma once

#include "PrimitiveComponent.h"

class ShapeComponent : public PrimitiveComponent {

public:
	ShapeComponent(Actor* _owner)  : PrimitiveComponent(_owner){
		isVisible = false;
		bCanCollide = true;
	}
};
