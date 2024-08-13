#pragma once

#include "Box.h"
#include "Circle.h"
#include "SimpleMath.h"

struct BoxCircleBounds {
	DXVec2 center;
	Extent2D boxExtent;
	float circleRadius;

	BoxCircleBounds() : 
		center{ 0.f, 0.f }, 
		boxExtent{ 0.f, 0.f }, 
		circleRadius{ 0.f } {}

	BoxCircleBounds(const Box& box) {
		center = box.GetCenter();
		boxExtent = box.GetExtent();
		circleRadius = 0.5f * sqrtf(boxExtent.width * boxExtent.width + boxExtent.height * boxExtent.height);
	}

	BoxCircleBounds(const Circle& circle) {
		center = circle.center;
		circleRadius = circle.radius;
		boxExtent = { circleRadius * 2.f, circleRadius * 2.f };
	}

	static bool CheckBoxesIntersect(const BoxCircleBounds& a, const BoxCircleBounds& b) {
		Box aBox = Box::BuildAABB(a.center, a.boxExtent);
		Box bBox = Box::BuildAABB(b.center, b.boxExtent);
		return aBox.CheckIntersect(bBox);
	}

	static bool CheckCirclesIntersect(const BoxCircleBounds& a, const BoxCircleBounds& b) {
		Circle aCircle = a.GetCircle();
		Circle bCircle = b.GetCircle();
		return aCircle.CheckIntersect(bCircle);
	}

	static bool CheckBoxesToPointIntersect(const BoxCircleBounds& a, const DXVec2& b) {
		Box aBox = Box::BuildAABB(a.center, a.boxExtent);
		
		return aBox.IsInside(b);
	}

	Box GetBox() const {
		return Box::BuildAABB(center, boxExtent);
	}

	Circle GetCircle() const {
		return Circle{ center, circleRadius };
	}
};
