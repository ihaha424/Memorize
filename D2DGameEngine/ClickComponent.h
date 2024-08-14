#pragma once

#include "IComponent.h"
#include "BoxCircleBounds.h"

class ClickComponent : public IComponent
{
	BoxCircleBounds*	ownerBounds = nullptr;
	bool				hover = false;
public:

public:
	ClickComponent(Actor* _owner);
	virtual ~ClickComponent() {};

	virtual void Update(float _dt);

	// TODO: Cursor Events
	void OnBeginCursorOver();
	void OnEndCursorOver();
	void OnClicked();
	void OnPressed();
};

template<class T>
concept ClickComponentType = std::is_base_of<ClickComponent, T>::value;