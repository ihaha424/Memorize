#pragma once

#include "framework.h"

struct Line {
	Math::Vector2 start;
	Math::Vector2 end;

	bool IntersectionPoint(const Line& other, Math::Vector2& outPoint) const {
		// NOTE: ChatGPT 4 Generated Code
		Math::Vector2 r = end - start;
		Math::Vector2 s = other.end - other.start;
		Math::Vector2 originDist = other.start - start;

		float RCrossS = r.x * s.y - r.y * s.x;
		float originDistCrossS = originDist.x * s.y - originDist.y * s.x;

		if (fabs(RCrossS) < EPSILON) { // Lines are parallel
			return false;
		}

		float t = originDistCrossS / RCrossS;

		if (t >= 0 && t <= 1) {
			outPoint = start + r * t;
			return true;
		}

		return false; // No intersection within line segments
	}

	/**
	 * @brief Check if the vector from the nearest point to the given point is perpendicular to this line.
	 * @param point 
	 * @return True if the vector is perpendicular, false otherwise.
	 */
	bool IsPerpendicularTo(const Math::Vector2& point) const {
		// NOTE: ChatGPT 4 Generated Code
		Math::Vector2 nearest = NearestPoint(point);
		Math::Vector2 diff = point - nearest;
		Math::Vector2 lineVec = end - start;

		return fabs(diff.Dot(lineVec)) < EPSILON;
	}

	Math::Vector2 NearestPoint(const Math::Vector2& point) const {
		// NOTE: ChatGPT 4 Generated Code
		Math::Vector2 lineVec = end - start;
		Math::Vector2 pointVec = point - start;

		float lineLengthSquared = lineVec.LengthSquared();
		if (lineLengthSquared < EPSILON) return start; // The line segment is too short

		float t = pointVec.Dot(lineVec) / lineLengthSquared;
		t = fmax(0, fmin(1, t)); // Clamp t to the segment
		return start + lineVec * t;
	}

	float DistanceSquaredTo(const Math::Vector2& point) const {
		Math::Vector2 nearest = NearestPoint(point);
		return (point - nearest).LengthSquared();
	}

	float DistanceTo(const Math::Vector2& point) const {
		Math::Vector2 nearest = NearestPoint(point);
		return (point - nearest).Length();
	}

	int WhichSide(const Math::Vector2& point) const {
		// NOTE: ChatGPT 4 Generated Code
		Math::Vector2 lineVec = end - start;
		Math::Vector2 pointVec = point - start;
		float det = lineVec.x * pointVec.y - lineVec.y * pointVec.x;

		if (fabs(det) < EPSILON) return 0; // On the line
		return det > 0 ? 1 : -1; // Right side is 1, left side is -1
	}
};
