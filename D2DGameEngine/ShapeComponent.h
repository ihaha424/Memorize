#pragma once

#include "PrimitiveComponent.h"

class ShapeComponent : public PrimitiveComponent {

public:

	ShapeComponent() {
		isVisible = false;
		bCanCollide = true;
	}


};
