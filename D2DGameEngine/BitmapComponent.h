#pragma once

#include "PrimitiveComponent.h"
#include "SpriteResource.h"
#include "ResourceManager.h"

// NOTE: 만약에 비트맵 컴포넌트에서 콜리션 체크하고 싶으면.
// bCanCollide = true
// CollisionProperty 세팅
// bool GetCollisionShape(float, CollisionShape&) const 오버로드해서 콜리션 기하 세팅

class BitmapComponent : public PrimitiveComponent {
	using Super = SceneComponent;

	D2D_RectF frame;
	std::shared_ptr<SpriteResource> sprite;

	Math::Matrix imageTransform;

public:

	BitmapComponent(Actor* _owner) :
		PrimitiveComponent(_owner),
		frame{ 0.f, 0.f, 0.f, 0.f },
		sprite{ nullptr },
		imageTransform{ Math::Matrix::Identity } {
		AddTickProperties(TICK_RENDER);
	}

	virtual ~BitmapComponent() {
		// TODO: 스프라이트 릴리즈
	}

	void SetSprite(const wchar_t* path) {
		sprite = ResourceManager::LoadResource<SpriteResource>(path);
		if (!sprite->GetResource()) {
			throw std::exception("BitmapScene: image asset is not found!");
		}
		// Fit the frame with the image width and height
		frame = {
			0,0,
			GetSpriteWidth(),
			GetSpriteHeight()
		};
	}

	std::shared_ptr<SpriteResource> GetSprite() { return sprite; }

	float GetSpriteWidth() const {
		return sprite->GetResource()->GetSize().width;
	}

	float GetSpriteHeight() const {
		return sprite->GetResource()->GetSize().height;
	}

	float GetFrameWidth() const {
		return frame.right - frame.left;
	}

	float GetFrameHeight() const {
		return frame.top - frame.bottom;
	}

	void SetFrame(const D2D_Point2F& ul, const D2D_Point2F& lr) {
		frame = { ul.x, ul.y, lr.x, lr.y };
	}

	void FlipX(bool flip) {
		imageTransform._11 = flip ? -1.f : 1.f;
	}

	void FlipY(bool flip) {
		imageTransform._22 = flip ? -1.f : 1.f;
	}

	virtual D2D_RectF GetCurrFrame() { return frame; }
	virtual Math::Matrix GetWorldTransform() const override;
	virtual void Render(class D2DRenderer* _renderer) override;

	// From Primitive Component
	virtual bool IsCollisionEnabled() const {
		return collisionProperty.collisionEnabled == CollisionEnabled::EnableCollision;
	}

	virtual CollisionEnabled::Type GetCollisionEnabled() const override {
		return collisionProperty.collisionEnabled;
	}

	virtual ECollisionChannel GetCollisionObjectType() const override {
		return collisionProperty.objectType;
	}

	/**
	 * @brief 콜리션 도형을 리턴합니다. 콜리션 도형은 컴포넌트의 스케일 값을 반영해야 합니다.
	 * @param Inflation
	 * @param CollisionShape
	 * @return
	 */
	virtual bool GetCollisionShape(float inflation, CollisionShape& collisionShape) const {
		// Extract the world scale.
		Math::Matrix worldMatrix = GetWorldTransform();
		Math::Vector3 scale = Math::ExtractScale(worldMatrix);
		Math::Matrix scaleMatrix = Math::Matrix::CreateScale(scale);

		std::initializer_list<Math::Vector2> vertices{
			{	frame.left, frame.top	},
			{ frame.right, frame.top },
			{ frame.right, frame.bottom },
			{	frame.left, frame.bottom	}
		};

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
		std::initializer_list<Math::Vector2> vertices{
			{	frame.left, frame.top	},
			{ frame.right, frame.top },
			{ frame.right, frame.bottom },
			{	frame.left, frame.bottom	}
		};
		collisionShape.SetPolygon(vertices);
		return collisionShape.IsNearlyZero();
	}

	virtual BoxCircleBounds CalculateBounds(const Math::Matrix& _worldTransform) const override {
		Math::Vector3 translate = Math::ExtractTranslation(_worldTransform);
		Math::Matrix invertTranslate = Math::Matrix::CreateTranslation(translate).Invert();
		Math::Matrix scaleAndRotation = _worldTransform * invertTranslate;
		
		float halfWidth = GetFrameWidth() / 2.f;
		float halfHeight = GetFrameHeight() / 2.f;

		std::vector<Math::Vector2> vertices{
			{ -halfWidth, -halfHeight },
			{ halfWidth, -halfHeight },
			{ halfWidth, halfHeight },
			{ -halfWidth, halfHeight },
		};

		for (Math::Vector2& v : vertices) {
			v = DXVec2::Transform(v, _worldTransform);
		}

		TPolygon polygon{ vertices };

		return BoxCircleBounds(polygon.GetAABB());
	}

	virtual BoxCircleBounds CalculateLocalBounds() const {
		
		float halfWidth = GetFrameWidth() / 2.f;
		float halfHeight = GetFrameHeight() / 2.f;

		std::vector<Math::Vector2> vertices{
			{ -halfWidth, -halfHeight },
			{ halfWidth, -halfHeight },
			{ halfWidth, halfHeight },
			{ -halfWidth, halfHeight },
		};

		for (Math::Vector2& v : vertices) {
			v = DXVec2::Transform(v, S * R * T);
		}

		TPolygon polygon{ vertices };

		return BoxCircleBounds(polygon.GetAABB());
	}

	virtual void UpdateBounds() override 
	{		
		bounds = CalculateBounds(GetWorldTransform());
	}
};

