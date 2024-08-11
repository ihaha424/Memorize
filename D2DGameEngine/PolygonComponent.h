#pragma once

#include "ShapeComponent.h"

#include "Polygon.h"

class PolygonComponent : public ShapeComponent {
protected:

	std::vector<Math::Vector2> vertices;

public:

	PolygonComponent(class Actor* _owner) : ShapeComponent(_owner) {}

	std::vector<Math::Vector2> GetScaledVertices() const {
		// Extract the world scale.
		Math::Matrix worldMatrix = GetWorldTransform();
		Math::Vector3 scale = Math::ExtractScale(worldMatrix);
		Math::Matrix scaleMatrix = Math::Matrix::CreateScale(scale);

		std::vector<Math::Vector2> scaledVertices;
		for (const Math::Vector2& v : vertices) {
			DXVec2 p{ v.x, v.y };
			p = DXVec2::Transform(p, scaleMatrix);
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
		// TODO: ¿ùµå Æ®·»½ºÆû Àû¿ë
		TPolygon polygon{ vertices };
		return BoxCircleBounds(polygon.GetAABB());
	}

	virtual bool GetCollisionShape(float inflation, CollisionShape& collisionShape) const {
		// Extract the world scale.
		Math::Matrix worldMatrix = GetWorldTransform();
		Math::Vector3 scale = Math::ExtractScale(worldMatrix);
		Math::Matrix scaleMatrix = Math::Matrix::CreateScale(scale);
		
		std::vector<Math::Vector2> inflatedVertices;
		for (const Math::Vector2& v : vertices) {
			DXVec2 p{ v.x, v.y };
			p = DXVec2::Transform(p, scaleMatrix);
			inflatedVertices.emplace_back(p.x * inflation, p.y * inflation);
		}
		collisionShape.SetPolygon(std::move(inflatedVertices));
		return true;
	}

	virtual bool IsZeroExtent() const {
		CollisionShape collisionShape;
		collisionShape.SetPolygon(vertices);
		return collisionShape.IsNearlyZero();
	}

	virtual bool CheckLineTraceComponent(
		HitResult& outHit,
		const DXVec2 start,
		const DXVec2 end) {
		// TODO:
		return false;
	}

	virtual bool CheckSweepComponent(
		HitResult& outHit,
		const DXVec2& start,
		const DXVec2& end,
		const DXMat4x4& rotation,
		const CollisionShape& collisionShape,
		const ECollisionChannel collisionChannel,
		const CollisionProperty& collisionProperty) override;

	virtual bool CheckOverlapComponent(
		OverlapResult& outOverlap,
		const DXVec2& pos,
		const DXMat4x4& rotation,
		const CollisionShape& collisionShape,
		const ECollisionChannel collisionChannel,
		const CollisionProperty& collisionProperty
	) override;

	virtual void Render(class D2DRenderer* _renderer) override;

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
