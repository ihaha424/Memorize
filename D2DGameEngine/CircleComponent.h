#pragma once

#include "ShapeComponent.h"

class CircleComponent : public ShapeComponent {
protected:

	float circleRadius{ 0.f };

public:

	CircleComponent(class Actor* _owner) : ShapeComponent(_owner) {}

	float GetShapeScale() const {
		// Extract the world scale.
		Math::Matrix worldMatrix = GetWorldTransform();
		Math::Vector3 scale = Math::ExtractScale(worldMatrix);

		return (std::min)({ scale.x, scale.y });
	}

	float GetScaledSphereRadius() const {
		return circleRadius * GetShapeScale();
	}

	void InitCircleRadius(float inCircleRadius) {
		circleRadius = inCircleRadius;
	}

	void SetCircleRadius(float inCircleRadius) {
		circleRadius = inCircleRadius;
	}

	virtual BoxCircleBounds CalculateLocalBounds() const override {
		// TODO: ¿ùµå Æ®·»½ºÆû Àû¿ë
		return BoxCircleBounds(Circle{ { 0.f, 0.f }, circleRadius });
	}

	virtual bool GetCollisionShape(float inflation, CollisionShape& collisionShape) const {
		collisionShape.SetCircle(GetScaledSphereRadius() * inflation);
		return true;
	}

	virtual bool IsZeroExtent() const {
		CollisionShape collisionShape;
		collisionShape.SetCircle(GetScaledSphereRadius());
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
		const CollisionProperty& collisionProperty
	) override;

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
