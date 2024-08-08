#pragma once

#include "ShapeComponent.h"

class BoxComponent : public ShapeComponent {
protected:
	Extent2D boxExtent{};

public:

	D2D_Vec2F GetScaledBoxExtent() const {
		D2D_Point2F p{ boxExtent.width, boxExtent.height };
		p = p * S;
		return { p.x, p.y };
	}

	D2D_Vec2F GetBoxExtent() const {
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
		return BoxCircleBounds(Box::BuildAABB({ 0, 0 }, boxExtent));
	}

	virtual bool GetCollisionShape(float inflation, CollisionShape& collisionShape) const {
		collisionShape.SetBox({ boxExtent.width * inflation, boxExtent.height * inflation });
		return true;
	}

	virtual bool IsZeroExtent() const {
		CollisionShape collisionShape;
		collisionShape.SetBox({ boxExtent.width, boxExtent.height });
		return collisionShape.IsNearlyZero();
	}
};
