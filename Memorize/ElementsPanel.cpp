#include "ElementsPanel.h"
#include "D2DGameEngine/UIImage.h"

ElementsPanel::ElementsPanel(World* _world) : UIPanel(_world)
{
	SetPosition(70, 240);

	UIImage* qBasic = CreateUI<UIImage>(L"Q_Basic");
	qBasic->SetSprite(L"TestResource/UI/Q_black.png");
	qBasic->SetPosition(0, -90);

	UIImage* wBasic = CreateUI<UIImage>(L"W_Basic");
	wBasic->SetSprite(L"TestResource/UI/W_black.png");
	wBasic->SetPosition(0, -30);

	UIImage* eBasic = CreateUI<UIImage>(L"E_Basic");
	eBasic->SetSprite(L"TestResource/UI/E_black.png");
	eBasic->SetPosition(0, 30);

	UIImage* rBasic = CreateUI<UIImage>(L"R_Basic");
	rBasic->SetSprite(L"TestResource/UI/R_black.png");
	rBasic->SetPosition(0, 90);
}

ElementsPanel::~ElementsPanel()
{
}
