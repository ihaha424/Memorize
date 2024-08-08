#pragma once

#include "ShapeComponent.h"

class CircleComponent : public ShapeComponent {
protected:

	float circleRadius{ 0.f };

public:

	float GetShapeScale() const {
		return (std::min)(S.m11, S.m22);
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
		return BoxCircleBounds(Circle{ { 0.f, 0.f }, circleRadius });
	}

	virtual bool GetCollisionShape(float inflation, CollisionShape& collisionShape) const {
		collisionShape.SetCircle(circleRadius * inflation);
		return true;
	}

	virtual bool IsZeroExtent() const {
		CollisionShape collisionShape;
		collisionShape.SetCircle(circleRadius);
		return collisionShape.IsNearlyZero();
	}
};
