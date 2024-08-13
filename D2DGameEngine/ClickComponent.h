#pragma once

#include "IComponent.h"
#include "BoxCircleBounds.h"

class ClickComponent : public IComponent
{
	BoxCircleBounds* OwnerBounds;
public:

public:
	ClickComponent(Actor* _owner) : IComponent(_owner) { SetTickProperties(TICK_UPDATE); };
	virtual ~ClickComponent() {};

	// TODO: Cursor Events
	void OnBeginCursorOver();
	void OnEndCursorOver();
	void OnClicked();
	void OnPressed();
};

template<class T>
concept ClickComponentType = std::is_base_of<ClickComponent, T>::value;