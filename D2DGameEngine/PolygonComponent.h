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
			DXVec2 p{ v.x, v.y };
			p = DXVec2::Transform(p, S);
			scaledVertices.emplace_back(p.x, p.y);
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
		// TODO: ���� Ʈ������ ����
		TPolygon polygon{ vertices };
		return BoxCircleBounds(polygon.GetAABB());
	}

	virtual bool GetCollisionShape(float inflation, CollisionShape& collisionShape) const {
		std::vector<Math::Vector2> inflatedVertices;
		for (const Math::Vector2& v : vertices) {
			DXVec2 p{ v.x, v.y };
			p = DXVec2::Transform(p, S);
			inflatedVertices.emplace_back(p.x * inflation, p.y * inflation);
		}
		collisionShape.SetPolygon(std::move(inflatedVertices));
		return true;
	}

	virtual bool IsZeroExtent() const {
		CollisionShape collisionShape;
		std::vector<Math::Vector2> inflatedVertices;
		for (const Math::Vector2& v : vertices) {
			inflatedVertices.emplace_back(v.x, v.y);
		}
		collisionShape.SetPolygon(std::move(inflatedVertices));
		return collisionShape.IsNearlyZero();
	}

protected:
	bool CheckComponentOverlapComponentImpl(
		PrimitiveComponent* primComp,
		const DXVec2& pos,
		const DXMat4x4& rotation) override;
	bool CheckComponentOverlapComponentWithResultImpl(
		PrimitiveComponent* primComp,
		const DXVec2& pos, const DXMat4x4& rotation,
		std::vector<OverlapResult>& outOverlap) override;
};
