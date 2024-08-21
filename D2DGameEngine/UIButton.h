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
	std::vector<std::function<void()>> onUnHoveredHandlers;
	std::vector<std::function<void()>> onPressedHandlers;

public:
	UIButton(class World* _world) : UIElement(_world) 
	{
		SetTickProperties(TICK_UPDATE);
	};
	virtual ~UIButton() {};

	virtual void Update(float _dt);

	void OnHovered();
	void OnUnHovered();
	void OnClicked();
	void OnPressed();

	void AddOnClickHandler(std::function<void()> handler)
	{
		onClickHandlers.push_back(handler);
	}

	void AddOnHoveredHandler(std::function<void()> handler)
	{
		onHoveredHandlers.push_back(handler);
	}

	void AddOnPressedHandler(std::function<void()> handler)
	{
		onPressedHandlers.push_back(handler);
	}

	void AddOnUnHoveredHandler(std::function<void()> handler)
	{
		onUnHoveredHandlers.push_back(handler);
	}
};

