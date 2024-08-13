#include "UIElement.h"
#include "UIPanel.h"

UIElement::UIElement(class World* _world) : Actor(_world)
{
}

UIElement::~UIElement()
{
}

void UIElement::SetPosition(int _x, int _y)
{
	position.x = _x;
	position.y = _y;
	if (ownerPanel)
	{
		position.x += ownerPanel->GetPosition().x;
		position.y += ownerPanel->GetPosition().y;
	}
}
