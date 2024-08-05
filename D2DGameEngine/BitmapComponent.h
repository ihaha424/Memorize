#pragma once

#include "SceneComponent.h"

class BitmapComponent : public SceneComponent {
	using Super = SceneComponent;

	Extent2D extent;
	D2D_RectF frame;
	D2D_Sprite* sprite;

	D2D_Mat3x2F imageTransform;

public:

	BitmapComponent() : 
		extent{ 0.f, 0.f },
		frame{ 0.f, 0.f, 0.f, 0.f },
		sprite{ nullptr },
		imageTransform{ D2D_Mat3x2F::Identity() } {
		SetTickProperties(TICK_RENDER);
	}

	virtual ~BitmapComponent() {
		// TODO: 스프라이트 릴리즈
	}

	void SetSprite(const wchar_t* path) {
		// TODO: 리소스 매니저 필요함

		// extent 사이즈를 스프라이트 사이즈로 맞춤

		// frame 사이즈를 스프라이트 사이즈로 맞춤

		//_imageAsset = ResourceManager::GetImageAsset(path);
		//if (!_imageAsset) {
		//	throw std::exception("BitmapScene: image asset is not found!");
		//}
		//// The extent matches the sprite size
		//_extent = {
		//	_imageAsset->GetSpriteWidth(),
		//	_imageAsset->GetSpriteHeight()
		//};
		//// Fit the frame with the image width and height
		//_frame = {
		//	_imageAsset->GetSpriteWidth(),
		//	_imageAsset->GetSpriteHeight()
		//};
	}

	D2D_Sprite* GetSprite() { return sprite; }

	float GetSpriteWidth() const {
		return sprite->GetSize().width;
	}

	float GetSpriteHeight() const {
		return sprite->GetSize().height;
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
	virtual D2D_Mat3x2F GetWorldTransform() const override;
	virtual void Render(class D2DRenderer* _renderer);
};

