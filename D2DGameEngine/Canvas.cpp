#include "Canvas.h"
#include "UIElement.h"
#include "World.h"

Canvas::Canvas(class World* _world) : Actor(_world)
{
	SetTickProperties(TICK_UPDATE | TICK_RENDER);
}

Canvas::~Canvas()
{
}


void Canvas::Update(float _dt)
{
	for (auto panel : panelList)
	{
		if (panel->GetStatus() == OS_ACTIVE)
			panel->Update(_dt);
	}
}

void Canvas::Render(D2DRenderer* _renderer)
{
	for (auto panel : panelList)
	{
		if(panel->GetStatus() == OS_ACTIVE)
			panel->Render(_renderer);
	}
}

