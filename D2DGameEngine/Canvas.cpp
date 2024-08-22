#include "Canvas.h"
#include "UIElement.h"
#include "World.h"

Canvas::Canvas(class World* _world) : Actor(_world)
{
	SetTickProperties(TICK_UPDATE | TICK_RENDER);
}

Canvas::~Canvas()
{
	for (auto panel : panelList)
	{
		delete panel;
		panel = nullptr;
	}
}


void Canvas::BeginPlay()
{
	__super::BeginPlay();
	for (auto panel : panelList)
	{
		if (panel->GetStatus() == OS_AWAKE)
			panel->BeginPlay();
	}
}

void Canvas::Update(float _dt)
{
	for (auto panel : panelList)
	{
		if (panel->GetStatus() == OS_ACTIVE)
			panel->Update(_dt);
	}
	std::sort(panelList.begin(), panelList.end(),
		[](UIPanel* panel1, UIPanel* panel2)
		{return panel1->GetZOrder() < panel2->GetZOrder(); });
}

void Canvas::Render(D2DRenderer* _renderer)
{
	for (auto panel : panelList)
	{
		if(panel->GetStatus() == OS_ACTIVE)
			panel->Render(_renderer);
	}
}

