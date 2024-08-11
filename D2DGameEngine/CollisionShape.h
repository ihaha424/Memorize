#pragma once

#include "framework.h"

#include <variant>

#include "Polygon.h"

enum class ECollisionShape {
	Line,
	Box,
	Circle,
	Capsule,
	Polygon
};

struct Circle_t {
	float radius;
};

struct Capsule_t {
	float radius;
	float halfHeight;
};

struct Box_t {
	float halfWidth;
	float halfHeight;
};

struct Line_t {
	float halfExtentX;
	float halfExtentY;
};

struct Polygon_t {
	std::vector<Math::Vector2> vertices;
};

using ShapeVariant = std::variant<Circle_t, Capsule_t, Box_t, Line_t, Polygon_t>;

struct CollisionShape {
	ECollisionShape shapeType;
	ShapeVariant shape;

	// Getters
	float GetCircleRadius() const {
		if (shapeType != ECollisionShape::Circle) {
			throw std::runtime_error("Not a circle.");
		}
		return std::get<Circle_t>(shape).radius;
	}

	float GetCapsuleRadius() const {
		if (shapeType != ECollisionShape::Capsule) {
			throw std::runtime_error("Not a capsule.");
		}
		return std::get<Capsule_t>(shape).radius;
	}

	float GetCapsuleHalfHeight() const {
		if (shapeType != ECollisionShape::Capsule) {
			throw std::runtime_error("Not a capsule.");
		}
		return std::get<Capsule_t>(shape).halfHeight;
	}

	Math::Vector2 GetBox() const {
		if (shapeType != ECollisionShape::Box) {
			throw std::runtime_error("Not a box.");
		}
		return { std::get<Box_t>(shape).halfWidth, std::get<Box_t>(shape).halfHeight };
	}

	Extent2D GetExtent() const {
		if (shapeType == ECollisionShape::Box) {
			return { std::get<Box_t>(shape).halfWidth * 2.f, std::get<Box_t>(shape).halfHeight * 2.f };
		}
		else if (shapeType == ECollisionShape::Capsule) {
			return { std::get<Capsule_t>(shape).radius * 2.f, std::get<Capsule_t>(shape).halfHeight * 2.f };
		}
		else if (shapeType == ECollisionShape::Circle) {
			return { std::get<Circle_t>(shape).radius * 2.f, std::get<Circle_t>(shape).radius * 2.f };
		}
		else if (shapeType == ECollisionShape::Polygon) {
			const auto& polygon = std::get<Polygon_t>(shape);
			TPolygon p{ polygon.vertices };
			return p.GetAABB().GetExtent();
		}
		else {
			return { std::get<Line_t>(shape).halfExtentX * 2.f, std::get<Line_t>(shape).halfExtentY * 2.f };
		}
	}

	std::vector<Math::Vector2> GetPolygonVertices() const {
		if (shapeType != ECollisionShape::Polygon) {
			throw std::runtime_error("Not a polygon.");
		}
		return std::get<Polygon_t>(shape).vertices;
	}

	// Setters
	void SetCircle(float _radius) {
		shapeType = ECollisionShape::Circle;
		shape = Circle_t{ _radius };
	}

	void SetCapsule(float _radius, float _halfExtent) {
		shapeType = ECollisionShape::Capsule;
		shape = Capsule_t{ 
			.radius = _radius, 
			.halfHeight = _halfExtent 
		};
	}

	void SetBox(const Extent2D& _halfExtent) {
		shapeType = ECollisionShape::Box;
		shape = Box_t{
			.halfWidth = _halfExtent.width,
			.halfHeight = _halfExtent.height
		};
	}

	void SetCapsule(const Extent2D& _extent) {
		shapeType = ECollisionShape::Capsule;
		shape = Capsule_t{
			.radius = _extent.width,
			.halfHeight = _extent.height
		};
	}

	void SetPolygon(const std::vector<Math::Vector2>& _vertices) {
		shapeType = ECollisionShape::Polygon;
		shape = Polygon_t{
			.vertices = _vertices,
		};
	}

	void SetPolygon(std::vector<Math::Vector2>&& _vertices) noexcept {
		shapeType = ECollisionShape::Polygon;
		shape = Polygon_t{
			.vertices = std::move(_vertices),
		};
	}

	// Type checkers
	bool IsBox() const {
		return shapeType == ECollisionShape::Box;
	}

	bool IsCapsule() const {
		return shapeType == ECollisionShape::Capsule;
	}

	bool IsLine() const {
		return shapeType == ECollisionShape::Line;
	}

	bool IsCircle() const {
		return shapeType == ECollisionShape::Circle;
	}

	bool IsPolygon() const {
		return shapeType == ECollisionShape::Polygon;
	}

	bool IsNearlyZero() const {
		if (std::holds_alternative<Line_t>(shape)) {
			const auto& line = std::get<Line_t>(shape);
			return abs(line.halfExtentX) < EPSILON && abs(line.halfExtentY) < EPSILON;
		}
		else if (std::holds_alternative<Box_t>(shape)) {
			const auto& box = std::get<Box_t>(shape);
			return abs(box.halfHeight) < EPSILON && abs(box.halfWidth) < EPSILON;
		}
		else if (std::holds_alternative<Circle_t>(shape)) {
			const auto& circle = std::get<Circle_t>(shape);
			return abs(circle.radius) < EPSILON;
		}
		else if (std::holds_alternative<Capsule_t>(shape)) {
			const auto& capsule = std::get<Capsule_t>(shape);
			return abs(capsule.halfHeight) < EPSILON && abs(capsule.radius) < EPSILON;
		}
		else if (std::holds_alternative<Polygon_t>(shape)) {
			const auto& polygon = std::get<Polygon_t>(shape);
			TPolygon p{ polygon.vertices };
			Extent2D extent = p.GetAABB().GetExtent();
			return abs(extent.width / 2.f) < EPSILON && abs(extent.height / 2.f) < EPSILON;
		}

		return false;
	}

	// Factory methods to create specific shapes
	static CollisionShape CreateCircle(float _radius) {
		CollisionShape shape;
		shape.SetCircle(_radius);
		return shape;
	}

	static CollisionShape CreateCapsule(float _radius, float _halfHeight) {
		CollisionShape shape;
		shape.SetCapsule(_radius, _halfHeight);
		return shape;
	}

	static CollisionShape CreateBox(const Extent2D& _halfExtent) {
		CollisionShape shape;
		shape.SetBox(_halfExtent);
		return shape;
	}

	static CollisionShape CreatePolygon(const std::vector<Math::Vector2>& _vertices) {
		CollisionShape shape;
		shape.SetPolygon(_vertices);
		return shape;
	}

};
