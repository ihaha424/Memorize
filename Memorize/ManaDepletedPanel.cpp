#include "ManaDepletedPanel.h"
#include "D2DGameEngine/UIText.h"

ManaDepletedPanel::ManaDepletedPanel(World* _world) : UIPanel(_world)
{
	text = CreateUI<UIText>(L"ManaDepletedPanelText");
	text->SetColor(D2D_Color::White);
	text->SetWeight(FontWeight::Bold);
	text->SetFontSize(40);
	text->SetText(L"마나가 부족합니다");
	text->SetPosition(1920/2 + 90, 900);
	text->SetSize(500, 200);

}

ManaDepletedPanel::~ManaDepletedPanel()
{
}

void ManaDepletedPanel::BeginPlay()
{
	__super::BeginPlay();
	Inactivate();
}

void ManaDepletedPanel::Update(float _dt)
{
	__super::Update(_dt);
	elapsedTime += _dt;
	if (elapsedTime > duration)
	{
		elapsedTime = 0.f;
		Inactivate();
	}
}
