#pragma once

#include "ShapeComponent.h"

#include "Polygon.h"

class PolygonComponent : public ShapeComponent {
protected:

	std::vector<Math::Vector2> vertices;

public:

	std::vector<Math::Vector2> GetScaledVertices() const {
		std::vector<Math::Vector2> scaledVertices;
		for (const Math::Vector2& v : vertices) {
			D2D_Point2F p{ v.x, v.y };
			p = p * S;
			scaledVertices.push_back({ p.x, p.y });
		}
		return scaledVertices;
	}

	std::vector<Math::Vector2> GetUnscaledVertices() const {
		return vertices;
	}

	void InitPolygon(std::initializer_list<Math::Vector2> inVertices) {
		vertices = inVertices;
	}

	void SetPolygon(std::initializer_list<Math::Vector2> inVertices) {
		vertices = inVertices;
	}

	virtual BoxCircleBounds CalculateLocalBounds() const override {
		TPolygon polygon{ vertices };
		return BoxCircleBounds(polygon.GetAABB());
	}

	virtual bool GetCollisionShape(float inflation, CollisionShape& collisionShape) const {
		std::vector<Math::Vector2> inflatedVertices;
		for (const Math::Vector2& v : vertices) {
			inflatedVertices.emplace_back(v.x * inflation, v.y * inflation);
		}
		collisionShape.SetPolygon(inflatedVertices);
		return true;
	}

	virtual bool IsZeroExtent() const {
		CollisionShape collisionShape;
		std::vector<Math::Vector2> inflatedVertices;
		for (const Math::Vector2& v : vertices) {
			inflatedVertices.emplace_back(v.x, v.y);
		}
		collisionShape.SetPolygon(inflatedVertices);
		return collisionShape.IsNearlyZero();
	}
};
