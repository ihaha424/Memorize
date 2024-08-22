#pragma once
#include "D2DGameEngine/UIImage.h"
class CursorImage : public UIImage
{
public:
	CursorImage(World* _world);
	virtual ~CursorImage();
	virtual void Render(D2DRenderer* _renderer) override;

};

