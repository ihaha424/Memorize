#pragma once
#include "UIElement.h"

class UIImage : public UIElement
{
	D2D_Sprite* sprite;
public:
	UIImage();
	virtual ~UIImage();

	void SetSprite(D2D_Sprite* _sprite) { sprite = _sprite; }

	void Render(class D2DRenderer* _renderer);
};

