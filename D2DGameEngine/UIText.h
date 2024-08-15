#pragma once
#include "UIElement.h"
#include "D2DRenderer.h"

class UIText : public UIElement
{
	std::wstring text = L"";
	TextFormatInfo textFormatInfo;
	D2D_Color color = D2D_Color::Black;
public:
	UIText(class World* _world);
	virtual ~UIText() {};

	void SetText(std::wstring _text) { text = _text; }
	void SetFont(std::wstring _font) { textFormatInfo.fontFamilyName = _font.c_str(); }
	void SetFontSize(float _size) { textFormatInfo.fontSize = _size; }
	void SetStretch(FontStretch _stretch) { textFormatInfo.fontStretch = _stretch; }
	void SetStyle(FontStyle _style) { textFormatInfo.fontStyle = _style; }
	void SetWeight(FontWeight _weight) { textFormatInfo.fontWeight = _weight; }
	void SetColor(D2D_Color _color) { color = _color; }

	virtual void Render(D2DRenderer* _renderer) override;
};

