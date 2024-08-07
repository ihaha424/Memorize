#pragma once

#include "Ray.h"
#include "Line.h"
#include "Box.h"
#include "Circle.h"
#include "Capsule.h"
#include "Polygon.h"

namespace intersectionUtil {

	inline
	bool LineLineIntersect(const Line& l1, const Line& l2) {
		// TODO
		return false;
	}

	inline
	bool LineBoxIntersect(const Line& line, const Box& box) {
		// TODO
		return false;
	}

	inline
	bool LineCircleIntersect(const Line& line, const Circle& circle) {
		// TODO
		return false;
	}

	inline
	bool LineCapsuleIntersect(const Line& line, const Capsule& capsule) {
		// TODO
		return false;
	}

	inline
	bool LinePolygonIntersect(const Line& line, const TPolygon& polygon) {
		// TODO
		return false;
	}

	inline
	bool BoxBoxIntersect(const Box& box1, const Box& box2) {
		// TODO
		return false;
	}

	inline
	bool BoxCircleIntersect(const Box& box, const Circle& circle) {
		// TODO
		return false;
	}

	inline
	bool BoxCapsuleIntersect(const Box& box, const Capsule& capsule) {
		// TODO
		return false;
	}

	inline
	bool BoxPolygonIntersect(const Box& box, const TPolygon& polygon) {
		// TODO
		return false;
	}

	inline
	bool CircleCircleIntersect(const Circle& circle1, const Circle& circle2) {
		// TODO
		return false;
	}

	inline
	bool CircleCapsuleIntersect(const Circle& circle, const Capsule& capsule) {
		// TODO
		return false;
	}

	inline
	bool CirclePolygonIntersect(const Circle& circle, const TPolygon& polygon) {
		// TODO
		return false;
	}

	inline
	bool CapsuleCapsuleIntersect(const Capsule& capsule1, const Capsule& capsule2) {
		// TODO
		return false;
	}

	inline
	bool CapsulePolygonIntersect(const Capsule& capsule, const TPolygon& polygon) {
		// TODO
		return false;
	}

	inline
	bool PolygonPolygonIntersect(const TPolygon polygon1, const TPolygon polygon2) {
		// TODO
		return false;
	}

}






