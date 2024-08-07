#pragma once

#include "framework.h"

#include "Box.h"

struct Circle {
	DXVec2 center;
	float radius;

	float Diameter() const { return radius * 2.f; }

	bool CheckIntersect(const Circle& other) const {
		float r = radius + other.radius;
		return DXVec2::DistanceSquared(center, other.center) <= r * r;
	}

	float DistanceTo(const Math::Vector2& point) const {
		return (point - center).Length();
	}

	float DistanceSquaredTo(const Math::Vector2& point) const {
		return (point - center).LengthSquared();
	}

	bool IsInside(const DXVec2& point) const {
		return DXVec2::DistanceSquared(center, point) <= radius * radius;
	}

	Box GetAABB() const {
		Extent2D extent{ Diameter(), Diameter() };
		return Box::BuildAABB(center, extent);
	}
};
