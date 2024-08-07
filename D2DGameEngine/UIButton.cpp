#include "UIButton.h"
#include "Mouse.h"

void UIButton::Update(float _dt)
{
    UIElement::Update(_dt);

    D2D_Vec2F mousePos = {Mouse::curMousePosition.x, Mouse::curMousePosition.y};
    int left = position.x - width / 2;
    int top = position.y - height / 2;
    int right = position.x + width / 2;
    int buttom = position.y + height / 2;

    if (left < mousePos.x && mousePos.x < right && top < mousePos.y && mousePos.y < buttom)
    {

        if (Mouse::clickStaus[0] == InputState::KeyDown)
        {
            _state = BS_Clicked;
            OnClicked();
        }
        else if (Mouse::clickStaus[0] == InputState::KeyHold)
        {
            _state = BS_Pressed;
            OnPressed();
        }
        else
        {
            _state = BS_Hovered;
            OnHovered();
        }
    }
    else
    {
        _state = BS_None;
    }
}

void UIButton::OnHovered()
{
    for (auto handler : onHoveredHandlers)
    {
        handler();
    }
}

void UIButton::OnClicked()
{
    for (auto handler : onClickHandlers)
    {
        handler();
    }
}

void UIButton::OnPressed()
{
    for (auto handler : onPressedHandlers)
    {
        handler();
    }
}
