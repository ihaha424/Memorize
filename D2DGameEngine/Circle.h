#pragma once

#include "framework.h"

struct Circle {
	DXVec2 center;
	float radius;

	bool CheckIntersect(const Circle& other) const {
		float r = radius + other.radius;
		return DXVec2::DistanceSquared(center, other.center) <= r * r;
	}

	bool IsInside(const DXVec2& point) {
		return DXVec2::DistanceSquared(center, point) <= radius * radius;
	}
};
