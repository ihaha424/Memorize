#pragma once

#include "framework.h"

enum class ECollisionShape {
	Line,
	Box,
	Circle,
	Capsule,
};

struct CollisionShape {
	ECollisionShape shapeType;

	union {
		struct {  // Circle with a center at local origin
			float radius;
		} Circle;

		struct {  // Capsule along the Y-axis from -halfHeight to +halfHeight with the given radius
			float radius;
			float halfHeight;
		} Capsule;

		struct {  // Axis-aligned box with the given width and height centered at the local origin
			float halfWidth;
			float halfHeight;
		} Box;

		struct {  // Line segment from Start to End (Not commonly used for stationary collision shapes)
			float halfExtentX;
			float halfExtentY;
		} Line;
	};

	// Getters
	float GetSphereRadius() const {
		if (shapeType != ECollisionShape::Circle) {
			throw std::runtime_error("Not a circle.");
		}
		return Circle.radius;
	}

	float GetCapsuleRadius() const {
		if (shapeType != ECollisionShape::Capsule) {
			throw std::runtime_error("Not a capsule.");
		}
		return Capsule.radius;
	}

	float GetCapsuleHalfHeight() const {
		if (shapeType != ECollisionShape::Capsule) {
			throw std::runtime_error("Not a capsule.");
		}
		return Capsule.halfHeight;
	}

	float GetCapsuleAxisHalfLength() const {
		return GetCapsuleHalfHeight();
	}

	DXVec2 GetBox() const {
		if (shapeType != ECollisionShape::Box) {
			throw std::runtime_error("Not a box.");
		}
		return { Box.halfWidth, Box.halfHeight };
	}

	Extent2D GetExtent() const {
		if (shapeType == ECollisionShape::Box) {
			return { Box.halfWidth * 2.f, Box.halfHeight * 2.f };
		}
		else if (shapeType == ECollisionShape::Capsule) {
			return { Capsule.radius * 2.f, Capsule.halfHeight * 2.f };
		}
		else if (shapeType == ECollisionShape::Circle) {
			return { Circle.radius * 2.f, Circle.radius * 2.f };
		}
		else {
			return { Line.halfExtentX * 2.f, Line.halfExtentY * 2.f };
		}
	}

	// Setters
	void SetSphere(float _radius) {
		shapeType = ECollisionShape::Circle;
		Circle.radius = _radius;
	}

	void SetCapsule(float _radius, float _halfExtent) {
		shapeType = ECollisionShape::Capsule;
		Capsule.radius = _radius;
		Capsule.halfHeight = _halfExtent;
	}

	void SetBox(const Extent2D& _halfExtent) {
		shapeType = ECollisionShape::Box;
		Box.halfWidth = _halfExtent.width;
		Box.halfHeight = _halfExtent.height;
	}

	void SetCapsule(const Extent2D& _extent) {
		shapeType = ECollisionShape::Capsule;
		Capsule.radius = _extent.width / 2.f;
		Capsule.halfHeight = _extent.height / 2.f;
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

	bool IsNearlyZero() const {
		return abs(Line.halfExtentX) < 0.0005 && abs(Line.halfExtentY) < 0.0005;
	}

	// Factory methods to create specific shapes
	static CollisionShape CreateCircle(float radius) {
		CollisionShape shape;
		shape.shapeType = ECollisionShape::Circle;
		shape.Circle.radius = radius;
		return shape;
	}

	static CollisionShape CreateCapsule(float radius, float halfHeight) {
		CollisionShape shape;
		shape.shapeType = ECollisionShape::Capsule;
		shape.Capsule.radius = radius;
		shape.Capsule.halfHeight = halfHeight;
		return shape;
	}

	static CollisionShape CreateBox(const Extent2D& _halfExtent) {
		CollisionShape shape;
		shape.shapeType = ECollisionShape::Box;
		shape.Box.halfWidth = _halfExtent.width;
		shape.Box.halfHeight = _halfExtent.height;
		return shape;
	}

};
