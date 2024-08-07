#pragma once

#include "ShapeComponent.h"

class BoxComponent : public ShapeComponent {
protected:
	Extent2D boxExtent{};

public:

	D2D_Vec2F GetScaledBoxExtent() {
		D2D_Point2F p{ boxExtent.width, boxExtent.height };
		p = p * S;
		return { p.x, p.y };
	}

	D2D_Vec2F GetBoxExtent() {
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

	BoxCircleBounds CalculateLocalBounds() const override {
		return BoxCircleBounds(Box::BuildAABB({ 0, 0 }, boxExtent));
	}

	CollisionShape GetCollisionShape() const override {
		return CollisionShape::CreateBox({ boxExtent.width / 2.f, boxExtent.height / 2.f });
	}
};
