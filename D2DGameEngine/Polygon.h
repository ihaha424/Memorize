#pragma once

#include "framework.h"

struct TPolygon {
	uint numPoints;
	std::vector<Math::Vector2> points;

	TPolygon() : numPoints{ 0U } {}

	TPolygon(std::initializer_list<Math::Vector2> _points) :
		numPoints{ _points.size() } {
		// TODO: convex hull sorting

	}

	DXVec2 GetCenter() const {
		Math::Vector2 res{};
		for (const Math::Vector2& p : points) {
			res += p;
		}
		res /= numPoints;

		return res;
	}

	std::vector<Math::Vector2> GetVertices() {
		return points;
	}

	std::vector<Math::Vector2> GetAxisNormals() {

		std::vector<Math::Vector2> n(points.size());
		for (int i = 0, j = points.size() - 1; i < n.size(); j = i++) {
			Math::Vector2 axis = points[i] - points[j];
			n[i] = Math::Vector2{ -axis.y, axis.x };	// rotate -90 degree
			n[i].Normalize();
		}

		return n;
	}

};
