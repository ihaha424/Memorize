#include "Cursor.h"

#include "D2DGameEngine/SceneComponent.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/CameraComponent.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "D2DGameEngine/D2DRenderer.h"

#include "D2DGameEngine/Mouse.h"

Cursor::Cursor(World* _world) : Actor(_world)
{
	renderLayer = INT_MAX;

	SetTickProperties(TICK_UPDATE | TICK_RENDER);

	location = CreateComponent<SceneComponent>();
	rootComponent = location;

	arrow = CreateComponent<AnimationBitmapComponent>();
	location->AddChild(arrow);
	arrow->SetSprite(L"TestResource/Cursors/Cursor_Default.png");
	arrow->SliceSpriteSheet(61, 60, 0, 0, 0, 0);
	arrow->SetFrameDurations({ 1.f / 12.f });
	arrow->FrameResize(0);
	arrow->Trigger(true);
	arrow->SetLoop(true);
	arrow->isVisible = false;

	pointer = CreateComponent<AnimationBitmapComponent>();
	location->AddChild(pointer);
	pointer->SetSprite(L"TestResource/Cursors/Cursor_Pointer.png");
	pointer->SliceSpriteSheet(61, 60, 0, 0, 0, 0);
	pointer->SetFrameDurations({ 1.f / 12.f });
	pointer->FrameResize(0);
	pointer->Trigger(true);
	pointer->SetLoop(true);
	pointer->isVisible = false;

	click = CreateComponent<AnimationBitmapComponent>();
	location->AddChild(click);
	click->SetSprite(L"TestResource/Cursors/Cursor_Click.png");
	click->SliceSpriteSheet(61, 60, 0, 0, 0, 0);
	click->SetFrameDurations({ 1.f / 12.f });
	click->FrameResize(0);
	click->Trigger(true);
	click->SetLoop(true);
	click->isVisible = false;

	SetShape(CursorShape::Arrow);
}

void Cursor::SetShape(CursorShape _shape)
{
	shape = _shape;

	arrow->isVisible = false;
	pointer->isVisible = false;
	click->isVisible = false;

	switch (_shape)
	{
	case CursorShape::Arrow:
		arrow->isVisible = true;
		break;
	case CursorShape::Pointer:
		pointer->isVisible = true;
		break;
	case CursorShape::Click:
		click->isVisible = true;
		break;
	case CursorShape::NUM_SHAPES:
		break;
	default:
		break;
	}
}

void Cursor::Update(float _dt)
{
	if (Mouse::clickStaus[0])
	{
		SetShape(CursorShape::Click);
	}
	else
	{
		SetShape(CursorShape::Arrow);
	}
}

void Cursor::Render(D2DRenderer* _renderer)
{
	Math::Vector2 destPos = GetWorld()->ScreenToWorldPoint({ Mouse::curMousePosition.x, Mouse::curMousePosition.y });

	SetLocation(destPos.x, destPos.y);

	for (auto [_, primComp] : renderSequence)
	{
		if (primComp->isVisible &&
			primComp->CheckTickProperty(TICK_RENDER) &&
			primComp->GetStatus() == OS_ACTIVE)
			primComp->Render(_renderer);
	}
}
