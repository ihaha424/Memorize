#pragma once

#include "framework.h"

class Line {
	Math::Vector2 direction;
	Math::Vector2 origin;

	float DistanceSquared(const Math::Vector2& point) {
		return 0.f;
	}

};
