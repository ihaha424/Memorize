#include "UIText.h"
#include "D2DRenderer.h"

UIText::UIText(class World* _world) : UIElement(_world)
{
	SetTickProperties(TICK_RENDER);
	textFormatInfo.fontFamilyName = L"Arial";
	textFormatInfo.fontSize = 10.f;
	textFormatInfo.fontStretch = FontStretch::Normal;
	textFormatInfo.fontStyle = FontStyle::Normal;
	textFormatInfo.fontWeight = FontWeight::Normal;
}

void UIText::Render(D2DRenderer* _renderer)
{
	__super::Render(_renderer);

	_renderer->DrawString(
		text, 
		&textFormatInfo, 
		{ position.x - width/2, position.y - height/2 }, 
		{ position.x + width / 2, position.y + height / 2 },
		color);
}
