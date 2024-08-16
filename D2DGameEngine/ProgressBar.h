#pragma once
#include "UIElement.h"

enum class ProgressBarDir
{
	LeftToRight,
	RightToLeft,
	BottomToTop,
	TopToBottom
};

class ProgressBar : public UIElement
{
	D2D_Sprite* fullImage = nullptr;
	float value = 1.0f;
	ProgressBarDir dir = ProgressBarDir::BottomToTop;

public:
	ProgressBar(class World* _world);
	virtual ~ProgressBar();

	virtual void Render(D2DRenderer* _renderer) override;

	void SetValue(float _value) { value = _value; }
	void SetValueRef(const float& _value) { value = _value; }
	void SetDir(ProgressBarDir _dir) { dir = _dir; }
	void SetFillImage(D2D_Sprite* _image) { fullImage = _image; }
	void SetFillImage(std::wstring _path);
};

