#pragma once

#include "ShapeComponent.h"

#include "Capsule.h"

class CapsuleComponent : public ShapeComponent {
protected:

	float capsuleHalfHeight{ 0.f };
	float capsuleRadius{ 0.f };
	
public:

	CapsuleComponent(class Actor* _owner) : ShapeComponent(_owner) {}

	float GetShapeScale()  const {
		// Extract the world scale.
		Math::Matrix worldMatrix = GetWorldTransform();
		Math::Vector3 scale = Math::ExtractScale(worldMatrix);

		return (std::min)({ scale.x, scale.y });
	}

	float GetScaledCapsuleHalfHeight()  const {
		// Extract the world scale.
		Math::Matrix worldMatrix = GetWorldTransform();
		Math::Vector3 scale = Math::ExtractScale(worldMatrix);

		return capsuleHalfHeight * scale.y;
	}

	float GetScaledCapsuleRadius()  const {
		// Extract the world scale.
		Math::Matrix worldMatrix = GetWorldTransform();
		Math::Vector3 scale = Math::ExtractScale(worldMatrix);

		return capsuleRadius * scale.x;
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
			.direction = -DXVec2::UnitY,
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
