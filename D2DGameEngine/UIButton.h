#pragma once
#include "UIElement.h"

enum ButtonState
{
	BS_None,
	BS_Hovered,
	BS_Clicked,
	BS_Pressed
};

class UIButton : public UIElement
{
	ButtonState _state = BS_None;

	std::vector<std::function<void()>> onClickHandlers;
	std::vector<std::function<void()>> onHoveredHandlers;
	std::vector<std::function<void()>> onPressedHandlers;

public:
	UIButton(class World* _world) : UIElement(_world) {};
	virtual ~UIButton() {};

	virtual void Init();
	virtual void Update(float _dt);

	void OnHovered();
	void OnClicked();
	void OnPressed();

	void AddOnClickHandler(void (*handler)())
	{
		onClickHandlers.push_back(handler);
	}

	void AddOnHoveredHandler(void (*handler)())
	{
		onHoveredHandlers.push_back(handler);
	}

	void AddOnPressedHandler(void (*handler)())
	{
		onPressedHandlers.push_back(handler);
	}

};

