#pragma once

#include "framework.h"

#include "Box.h"

struct TPolygon {
	std::size_t numPoints;
	std::vector<Math::Vector2> points;

	TPolygon() : numPoints{ 0U } {}

	TPolygon(const std::vector<Math::Vector2>& _points) :
		numPoints{ _points.size() },
		points{ _points } {}

	TPolygon(std::initializer_list<Math::Vector2> _points) :
		numPoints{ _points.size() },
		points(_points) {
		// TODO: may sort the points clockwise, and find convex hull.
	}

	Math::Vector2 GetCenter() const {
		Math::Vector2 res{};
		for (const Math::Vector2& p : points) {
			res += p;
		}
		res /= numPoints;

		return res;
	}

	std::vector<Math::Vector2> GetVertices() const {
		return points;
	}

	std::vector<Math::Vector2> GetAxisNormals() const {

		std::vector<Math::Vector2> n(points.size());
		for (int i = 0, j = points.size() - 1; i < n.size(); j = i++) {
			Math::Vector2 axis = points[i] - points[j];
			n[i] = Math::Vector2{ -axis.y, axis.x };	// rotate -90 degree
			n[i].Normalize();
		}

		return n;
	}

	bool IsInside(const Math::Vector2& point) {
		// NOTE: ChatGPT 4 Generated Code
		bool inside = false;
		size_t n = points.size();
		for (size_t i = 0, j = n - 1; i < n; j = i++) {
			const Math::Vector2& pi = points[i]; // Current vertex
			const Math::Vector2& pj = points[j]; // Previous vertex

			// Check if point is in the y-range of the edge
			if ((pi.y > point.y) != (pj.y > point.y)) {
				// Calculate x-coordinate of the intersection of the line from pi to pj with the horizontal line through point.y
				float intersectX = (pj.x - pi.x) * (point.y - pi.y) / (pj.y - pi.y) + pi.x;

				// If point.x is to the left of intersectX, then the ray crosses the edge
				if (point.x < intersectX) {
					inside = !inside;
				}
			}
		}
		return inside;
	}

	Box GetAABB() const {
		// NOTE: ChatGPT 4 Generated Code
		if (points.empty()) return { {0, 0}, {0, 0} };

		float minX = points[0].x;
		float maxX = points[0].x;
		float minY = points[0].y;
		float maxY = points[0].y;

		for (const auto& p : points) {
			if (p.x < minX) minX = p.x;
			if (p.x > maxX) maxX = p.x;
			if (p.y < minY) minY = p.y;
			if (p.y > maxY) maxY = p.y;
		}

		return {
			{minX, maxY},
			{maxX, minY}
		};
	}

};
