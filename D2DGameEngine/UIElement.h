#pragma once
#include "Actor.h"

class UIElement : public Actor
{
protected:
	D2D_Vec2F position;
	int width = 100;
	int height = 100;

	bool bIsActive = true;
	int zOrder = 0;
public:
	UIElement();
	virtual ~UIElement();

	void SetPosition(int _x, int _y) { position.x = _x; position.y = _y; }
	D2D_Vec2F GetPosition() { return position; }

	void SetSize(int _width, int _height) { width = _width; height = _height; }
	D2D_Vec2F GetSize() { return { (float)width, (float)height }; }

	bool GetIsActive() { return bIsActive; }
	void SetIsActive(bool _active) { bIsActive = _active; }

	int GetZOrder() { return zOrder; }
	void SetZOrder(int _z) { zOrder = _z; }

};

