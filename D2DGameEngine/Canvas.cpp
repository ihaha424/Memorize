#include "Canvas.h"
#include "UIElement.h"
#include "World.h"

Canvas::Canvas()
{
}

Canvas::~Canvas()
{
}

void Canvas::AddToViewport()
{
	GetWorld()->AddUICanvas(this);
}

void Canvas::RemoveFromViewport()
{
	GetWorld()->RemoveUICanvas(this);
}

void Canvas::Update(float _dt)
{
	for (auto ui : uiList)
	{
		ui->Update(_dt);
	}
}

void Canvas::Render(D2DRenderer* _renderer)
{
	for (auto ui : uiList)
	{
		ui->Render(_renderer);
	}
}

