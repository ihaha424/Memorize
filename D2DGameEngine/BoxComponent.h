#pragma once

#include "ShapeComponent.h"

class BoxComponent : public ShapeComponent {
protected:
	Extent2D boxExtent{};

public:

	BoxComponent(class Actor* _owner) : ShapeComponent(_owner) {}

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

	virtual bool GetCollisionShape(float inflation, CollisionShape& collisionShape) const {
		Extent2D scaledExtent = GetScaledBoxExtent();
		collisionShape.SetBox({ 0.5f * scaledExtent.width * inflation, 0.5f * scaledExtent.height * inflation });
		return true;
	}

	virtual bool IsZeroExtent() const {
		CollisionShape collisionShape;
		collisionShape.SetBox({ boxExtent.width, boxExtent.height });
		return collisionShape.IsNearlyZero();
	}

	virtual BoxCircleBounds CalculateBounds(const Math::Matrix& _worldTransform) const override {

		Math::Vector2 location = GetComponentLocation();
		Extent2D extent = GetScaledBoxExtent();

		Box box{
			.ul = { location.x - extent.width / 2.f, location.y - extent.height / 2.f },
			.lr = { location.x + extent.width / 2.f, location.y + extent.height / 2.f }
		};

		return BoxCircleBounds(box);
	}

	virtual BoxCircleBounds CalculateLocalBounds() const override {
		Extent2D extent = GetScaledBoxExtent();

		Box box {
			.ul = { -extent.width / 2.f, -extent.height / 2.f },
			.lr = { +extent.width / 2.f, +extent.height / 2.f }
		};

		return BoxCircleBounds(box);
	}

	virtual void UpdateBounds() override
	{
		bounds = CalculateBounds(GetWorldTransform());
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
