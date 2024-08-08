#pragma once

#include "ShapeComponent.h"

class BoxComponent : public ShapeComponent {
protected:
	Extent2D boxExtent{};

public:

	Extent2D GetScaledBoxExtent() const {
		// Extract the world scale.
		Math::Matrix worldMatrix = GetWorldTransform();
		Math::Vector3 scale = Math::ExtractScale(worldMatrix);

		// Scale the height;
		return { boxExtent.width * scale.x, boxExtent.height * scale.y };
	}

	Extent2D GetBoxExtent() const {
		return { boxExtent.width, boxExtent.height };
	}

	void InitBoxExtent(const Extent2D& _boxExtent) {
		boxExtent = _boxExtent;
	}

	/**
	 * @brief 박스 사이즈를 바꿉니다.
	 * @param _boxExtent 
	 */
	void SetBoxExtent(const Extent2D& _boxExtent) {
		boxExtent = _boxExtent;
		// TODO: 콜리션 체크
	}

	virtual BoxCircleBounds CalculateLocalBounds() const override {
		// TODO: 월드 트렌스폼 적용
		return BoxCircleBounds(Box::BuildAABB({ 0, 0 }, boxExtent));
	}

	virtual bool GetCollisionShape(float inflation, CollisionShape& collisionShape) const {
		Extent2D scaledExtent = GetScaledBoxExtent();
		collisionShape.SetBox({ scaledExtent.width * inflation, scaledExtent.height * inflation });
		return true;
	}

	virtual bool IsZeroExtent() const {
		CollisionShape collisionShape;
		collisionShape.SetBox({ boxExtent.width, boxExtent.height });
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
