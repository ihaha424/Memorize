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

void UIPanel::BeginPlay()
{
	__super::BeginPlay();
	for (auto ui : uiList)
	{
		if (ui->GetStatus() == OS_AWAKE)
			ui->BeginPlay();
	}
}

void UIPanel::Update(float _dt)
{
	for (auto ui : uiList)
	{
		if (ui->GetStatus() == OS_ACTIVE)
			ui->Update(_dt);
	}
	uiList.sort([](UIElement* u1, UIElement* u2)
		{return u1->GetZOrder() < u2->GetZOrder(); });
}

void UIPanel::Render(D2DRenderer* _renderer)
{
	for (auto ui : uiList)
	{
		if(ui->GetStatus() == OS_ACTIVE)
			ui->Render(_renderer);
	}
}
