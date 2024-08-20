#include "UIPanel.h"

UIPanel::UIPanel(World* _world) : UIElement(_world)
{
	SetTickProperties(TICK_UPDATE | TICK_RENDER);
}

UIPanel::~UIPanel()
{
	for (auto ui : uiList)
	{
		delete ui;
		ui = nullptr;
	}
}

void UIPanel::Update(float _dt)
{
	for (auto ui : uiList)
	{
		ui->Update(_dt);
	}
}

void UIPanel::Render(D2DRenderer* _renderer)
{
	for (auto ui : uiList)
	{
		if(ui->GetStatus() == OS_ACTIVE)
			ui->Render(_renderer);
	}
}
