#pragma once

#include "ShapeComponent.h"

#include "Capsule.h"

class CapsuleComponent : public ShapeComponent {
protected:

	float capsuleHalfHeight{ 0.f };
	float capsuleRadius{ 0.f };
	
public:

	float GetShapeScale()  const {
		return sqrtf(S._11 * S._11 + S._22 * S._22);
	}

	float GetScaledCapsuleHalfHeight()  const {
		return capsuleHalfHeight * GetShapeScale();
	}

	float GetScaledCapsuleRadius()  const {
		return capsuleRadius * GetShapeScale();
	}

	float GetUnscaledCapsuleHalfHeight()  const {
		return capsuleHalfHeight;
	}

	float GetUnscaledCapsuleRadius()  const {
		return capsuleRadius;
	}

	void GetUnscaledCapsuleSize(float& outRadius, float& outHalfHeight)  const {
		outRadius = capsuleRadius;
		outHalfHeight = capsuleHalfHeight;
	}

	void InitCapsuleSize(float inRadius, float inHalfHeight) {
		capsuleRadius = inRadius;
		capsuleHalfHeight = inHalfHeight;
	}

	void SetCapsuleHalfHeight(float halfHeight) {
		capsuleHalfHeight = halfHeight;
	}

	void SetCapsuleRadius(float radius) {
		capsuleRadius = radius;
	}

	void SetCapsuleSize(float inRadius, float inHalfHeight) {
		capsuleRadius = inRadius;
		capsuleHalfHeight = inHalfHeight;
	}

	virtual BoxCircleBounds CalculateLocalBounds() const override {
		// TODO: ¿ùµå Æ®·»½ºÆû Àû¿ë
		Capsule capsule{
			.center = { 0.f, 0.f },
			.direction = { 0.f, 1.f },
			.extent = capsuleHalfHeight,
			.radius = capsuleRadius
		};
		return BoxCircleBounds(capsule.GetBox());
	}

	virtual bool GetCollisionShape(float inflation, CollisionShape& collisionShape) const {
		collisionShape.SetCapsule({ GetScaledCapsuleRadius() * inflation, GetScaledCapsuleHalfHeight() * inflation });
		return true;
	}

	virtual bool IsZeroExtent() const {
		CollisionShape collisionShape;
		collisionShape.SetCapsule({ capsuleRadius, capsuleHalfHeight });
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
