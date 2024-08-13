#pragma once
#include "UIElement.h"

class UIImage : public UIElement
{
	D2D_Sprite* sprite;
public:
	UIImage(class World* _world);
	virtual ~UIImage();

	void SetSprite(std::wstring _path);

	void Render(class D2DRenderer* _renderer);
};

