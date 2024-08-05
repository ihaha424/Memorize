#include "Canvas.h"
#include "UIElement.h"

Canvas::Canvas()
{
}

Canvas::~Canvas()
{
}

void Canvas::AddToViewport()
{
}

void Canvas::RemoveFromViewport()
{
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

