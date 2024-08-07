#pragma once

#include "framework.h"

struct Ray {
	Math::Vector2 direction;
	Math::Vector2 origin;

	static Ray RayFromPoints(const Math::Vector2& p0, const Math::Vector2& p1) {
		Ray ray;
		ray.origin = p0;
		ray.direction = (p1 - p0);
		ray.direction.Normalize();
		return ray;
	}

	bool IntersectionPoint(const Ray& other, Math::Vector2& outPoint) const {
		// NOTE: ChatGPT 4 Generated Code
		// The directions of the rays
		const Math::Vector2& dir1 = this->direction;
		const Math::Vector2& dir2 = other.direction;

		// The origins of the rays
		const Math::Vector2& o1 = this->origin;
		const Math::Vector2& o2 = other.origin;

		// We need to solve the parametric equations:
		// o1 + t * dir1 = o2 + s * dir2
		// Rearrange to:
		// t * dir1 - s * dir2 = o2 - o1

		float denominator = dir1.x * dir2.y - dir1.y * dir2.x;

		if (std::fabs(denominator) < EPSILON) { // Parallel or coincident lines
			return false; // No intersection
		}

		float dx = o2.x - o1.x;
		float dy = o2.y - o1.y;

		float t = (dx * dir2.y - dy * dir2.x) / denominator;
		outPoint = o1 + t * dir1;
		return true;
	}

	/**
	 * @brief Find the nearest point on the ray to the point.
	 * @param point 
	 * @return 
	 */
	Math::Vector2 NearestPoint(const Math::Vector2& point) const {
		// NOTE: ChatGPT 4 Generated Code
		// Project point onto the ray
		Math::Vector2 relativePoint = point - origin;
		float t = relativePoint.Dot(direction);
		return origin + direction * t;
	}

	float DistanceSquaredTo(const Math::Vector2& point) const {
		Math::Vector2 nearest = NearestPoint(point);
		return (point - nearest).LengthSquared();
	}

	float DistanceTo(const Math::Vector2& point) const {
		Math::Vector2 nearest = NearestPoint(point);
		return (point - nearest).Length();
	}

	/**
	 * @brief 1 if point is on the right side, -1 on the left side, 0 on the line
	 * @param point 
	 * @return 
	 */
	int WhichSide(const Math::Vector2& point) const {
		float t1 = (direction.x - origin.x) * (point.y - origin.y);
		float t2 = (direction.y - origin.y) * (point.x - origin.x);
		float t = t1 - t2;

		if (fabs(t) < EPSILON) return 0;
		return t > 0 ? -1 : 1;
	}

	Math::Vector2 Project(const Math::Vector2& point) const {
		Math::Vector2 a = direction;
		Math::Vector2 b = point - origin;

		float ADotB = a.Dot(b);
		float BDotB = b.Dot(b);
		float s = ADotB / BDotB;
		return s * b;
	}

	Math::Vector2 Reject(const Math::Vector2& point) const {
		return direction - Project(point);
	}

};
