#include "ClickComponent.h"
#include "World.h"
#include "Actor.h"
#include "Mouse.h"

ClickComponent::ClickComponent(Actor* _owner)
	: IComponent(_owner)
{
	SetTickProperties(TICK_UPDATE);
	ownerBounds = &(_owner->rootComponent->bounds);
}


void ClickComponent::Update(float _dt)
{
	D2D_Vec2F mousePos = { Mouse::curMousePosition.x, Mouse::curMousePosition.y };
	Math::Vector2 worldMousePos = GetWorld()->ScreenToWorldPoint({mousePos.x, mousePos.y });
	/*LOG_MESSAGE(dbg::text("MousePos: ", mousePos.x, ", ", mousePos.y));
	LOG_MESSAGE(dbg::text("WorldPosMouse: ", worldMousePos.x, ", ", worldMousePos.y));*/
	if (BoxCircleBounds::CheckBoxesToPointIntersect(*ownerBounds, worldMousePos))
	{
		hover = true;
        if (Mouse::clickStaus[0] == InputState::KeyDown)
            OnClicked();
        else if (Mouse::clickStaus[0] == InputState::KeyHold)
            OnPressed();
        else
			OnBeginCursorOver();
	}
	else
	{
		if (hover == true)
		{
			OnEndCursorOver();
			hover = false;
		}
	}
}

void ClickComponent::OnBeginCursorOver()
{
	GetOwner()->OnBeginCursorOver();
}

void ClickComponent::OnEndCursorOver()
{
	GetOwner()->OnEndCursorOver();
}

void ClickComponent::OnClicked()
{
	GetOwner()->OnClicked();
}

void ClickComponent::OnPressed()
{
	GetOwner()->OnPressed();
}
