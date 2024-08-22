#pragma once
#include "UIElement.h"

class UIImage : public UIElement
{
protected:
	D2D_Sprite* sprite;
	float opacity{1.f};
public:
	UIImage(class World* _world);
	virtual ~UIImage();

	void SetSprite(std::wstring _path);
	void SetSprite(D2D_Sprite* _sprite) { sprite = _sprite; }

	void SetOpacity(float _opacity) { opacity = _opacity; }

	void Render(class D2DRenderer* _renderer);
};

