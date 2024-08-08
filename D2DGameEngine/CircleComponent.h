#pragma once

#include "ShapeComponent.h"

class CircleComponent : public ShapeComponent {
protected:

	float circleRadius{ 0.f };

public:

	float GetShapeScale() const {
		return sqrtf(S._11 * S._11 + S._22 * S._22);
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
