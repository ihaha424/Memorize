#pragma once

#include "framework.h"

#include "Polygon.h"

struct Capsule {
	Math::Vector2 center;			// The center point of the capsule
	Math::Vector2 direction;   // The direction of the capsule
	float extent;		// The extent of the direction, which is the half of the total length
	float radius;		// The radius of the capsule

	const Math::Vector2& Center() const {
		return center;
	}

	const Math::Vector2& Direction() const {
		return direction;
	}

	float Extent() const { return extent; }
	float Radius() const { return radius; }

	float Length() const {
		// NOTE: ChatGPT 4 Generated Code
		return 2.f * (extent + radius);
	}

	float HalfLength() const {
		return extent + radius;
	}
	
	/**
	 * @brief Signed distance from point to capsule surface. 
	 * Points inside capsule return negative distance.
	 * @param point 
	 * @return 
	 */
	float SignedDistance(const Math::Vector2& point) const {
		// NOTE: ChatGPT 4 Generated Code
		Math::Vector2 toPoint = point - center;
		Math::Vector2 normDir = direction; normDir.Normalize();
		Math::Vector2 projection = toPoint.Dot(normDir) * normDir;
		float distToAxis = (toPoint - projection).Length();
		float axialDistance = (std::min)((std::max)(-extent, toPoint.Dot(normDir)), extent);

		if (abs(axialDistance) == extent) {
			Math::Vector2 endpoint = center + normDir * axialDistance;
			return (endpoint - point).Length() - radius;
		}

		return distToAxis - radius;
	}

	bool Contains(const Math::Vector2& point) const {
		// NOTE: ChatGPT 4 Generated Code
		return SignedDistance(point) <= 0;
	}

	Box GetBox() const {
		// NOTE: ChatGPT 4 Generated Code
		Math::Vector2 normDir = direction; normDir.Normalize();
		Math::Vector2 axis = normDir * extent;
		Math::Vector2 corner = Math::Vector2(radius, radius);

		Box boundingBox;
		boundingBox.ul = center - axis - corner;
		boundingBox.lr = center + axis + corner;

		return boundingBox;
	}

	TPolygon GetOrientedBox() const {
		// NOTE: ChatGPT 4 Generated Code
		TPolygon box;
		Math::Vector2 normDir = direction; normDir.Normalize();
		Math::Vector2 perpDir(-normDir.y, normDir.x);
		Math::Vector2 axis = normDir * extent;
		Math::Vector2 corner = perpDir * radius;

		box.points.push_back(center - axis + corner);
		box.points.push_back(center + axis + corner);
		box.points.push_back(center + axis - corner);
		box.points.push_back(center - axis - corner);
		box.numPoints = 4;

		return box;
	}
	

};
