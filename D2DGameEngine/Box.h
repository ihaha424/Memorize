#pragma once

#include "framework.h"

// NOTE: y direction is pointing up

struct Box {
	DXVec2 ul;
	DXVec2 lr;

	static Box BuildAABB(const DXVec2& origin, const Extent2D& extent) {
		return {	// y-axis points downward
			{ origin.x - extent.width / 2.f, origin.y - extent.height / 2.f },
			{ origin.x + extent.width / 2.f, origin.y + extent.height / 2.f }
		};
	}

	DXVec2 GetCenter() const {
		return { // y-axis points downward
			ul.x + (lr.x - ul.x) / 2.f, 
			ul.y + (lr.y - ul.y) / 2.f
		};
	}

	Extent2D GetExtent() const {
		// y-axis points downward
		return { lr.x - ul.x, lr.y - ul.y };
	}

	float GetWidth() const {
		return lr.x - ul.x;
	}

	float GetHeight() const {
		return lr.y - ul.y;
	}

	std::vector<DXVec2> GetVertices() const {
		return { ul, {lr.x, ul.y}, lr, {ul.x, lr.y} };
	}

	bool CheckIntersect(const Box& other) const {
		float a_xmin = ul.x;
		float a_xmax = lr.x;
		float a_ymin = ul.y;
		float a_ymax = lr.y;

		float b_xmin = other.ul.x;
		float b_xmax = other.lr.x;
		float b_ymin = other.ul.y;
		float b_ymax = other.lr.y;

		return !(a_xmax < b_xmin || a_xmin > b_xmax || a_ymax < b_ymin || a_ymin > b_ymax);
	}

	bool IsInside(const DXVec2& point) const {
		return ul.x <= point.x && lr.x <= point.x && 
			ul.y <= point.y && point.y <= lr.y;
	}
};

