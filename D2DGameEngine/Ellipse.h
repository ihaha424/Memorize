#pragma once

#include "framework.h"

#include "Box.h"

struct TEllipse {
	DXVec2 center;
	float minor;
	float major;

	float SemiMinorAxis() const { return minor * 0.5f; }
	float SemiMajorAxis() const { return major * 0.5f; }

	bool CheckIntersectWithBox(const Box& box) const {
		// 1. Check intersection with the left edge (x = upperLeft.x)
		if (center.x - SemiMajorAxis() <= box.ul.x && center.x + SemiMajorAxis() >= box.ul.x) {
			float yDelta = SemiMinorAxis() * std::sqrt(1 - std::pow((box.ul.x - center.x) / SemiMajorAxis(), 2));
			float y1 = center.y + yDelta;
			float y2 = center.y - yDelta;
			if ((y1 >= box.ul.y && y1 <= box.lr.y) || (y2 >= box.ul.y && y2 <= box.lr.y)) {
				return true;
			}
		}

		// 2. Check intersection with the right edge (x = lowerRight.x)
		if (center.x - SemiMajorAxis() <= box.lr.x && center.x + SemiMajorAxis() >= box.lr.x) {
			float yDelta = SemiMinorAxis() * std::sqrt(1 - std::pow((box.lr.x - center.x) / SemiMajorAxis(), 2));
			float y1 = center.y + yDelta;
			float y2 = center.y - yDelta;
			if ((y1 >= box.ul.y && y1 <= box.lr.y) || (y2 >= box.ul.y && y2 <= box.lr.y)) {
				return true;
			}
		}

		// 3. Check intersection with the top edge (y = upperLeft.y)
		if (center.y - SemiMinorAxis() <= box.ul.y && center.y + SemiMinorAxis() >= box.ul.y) {
			float xDelta = SemiMajorAxis() * std::sqrt(1 - std::pow((box.ul.y - center.y) / SemiMinorAxis(), 2));
			float x1 = center.x + xDelta;
			float x2 = center.x - xDelta;
			if ((x1 >= box.ul.x && x1 <= box.lr.x) || (x2 >= box.ul.x && x2 <= box.lr.x)) {
				return true;
			}
		}

		// 4. Check intersection with the bottom edge (y = lowerRight.y)
		if (center.y - SemiMinorAxis() <= box.lr.y && center.y + SemiMinorAxis() >= box.lr.y) {
			float xDelta = SemiMajorAxis() * std::sqrt(1 - std::pow((box.lr.y - center.y) / SemiMinorAxis(), 2));
			float x1 = center.x + xDelta;
			float x2 = center.x - xDelta;
			if ((x1 >= box.ul.x && x1 <= box.lr.x) || (x2 >= box.ul.x && x2 <= box.lr.x)) {
				return true;
			}
		}

		// No intersection
		return false;
	}

	/*float DistanceTo(const Math::Vector2& point) const {
		return (point - center).Length();
	}

	float DistanceSquaredTo(const Math::Vector2& point) const {
		return (point - center).LengthSquared();
	}

	bool IsInside(const DXVec2& point) const {
		return DXVec2::DistanceSquared(center, point) <= radius * radius;
	}*/

	Box GetAABB() const {
		Extent2D extent{ major, minor };
		return Box::BuildAABB(center, extent);
	}
};
