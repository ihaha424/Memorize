#include "ElementsPanel.h"
#include "D2DGameEngine/UIImage.h"

ElementsPanel::ElementsPanel(World* _world) : UIPanel(_world)
{
	SetPosition(70, 240);

	q = CreateUI<UIImage>(L"Q");
	q->SetSprite(L"TestResource/UI/Q_black.png");
	q->SetPosition(0, -90);

	w = CreateUI<UIImage>(L"W");
	w->SetSprite(L"TestResource/UI/W_black.png");
	w->SetPosition(0, -30);

	e = CreateUI<UIImage>(L"E");
	e->SetSprite(L"TestResource/UI/E_black.png");
	e->SetPosition(0, 30);

	r = CreateUI<UIImage>(L"R");
	r->SetSprite(L"TestResource/UI/R_black.png");
	r->SetPosition(0, 90);
}

ElementsPanel::~ElementsPanel()
{
}

void ElementsPanel::SelectQ()
{
	q->SetSprite(L"TestResource/UI/Q.png");
	UnselectW();
	UnselectE();
	UnselectR();
}

void ElementsPanel::SelectW()
{
	w->SetSprite(L"TestResource/UI/W.png");
	UnselectQ();
	UnselectE();
	UnselectR();
}

void ElementsPanel::SelectE()
{
}

void ElementsPanel::SelectR()
{
}

void ElementsPanel::UnselectQ()
{
	q->SetSprite(L"TestResource/UI/Q_black.png");
}

void ElementsPanel::UnselectW()
{
	w->SetSprite(L"TestResource/UI/W_black.png");
}

void ElementsPanel::UnselectE()
{
	e->SetSprite(L"TestResource/UI/E_black.png");
}

void ElementsPanel::UnselectR()
{
	r->SetSprite(L"TestResource/UI/R_black.png");
}

void ElementsPanel::OnPressedQ()
{
	SelectQ();
}
void ElementsPanel::OnPressedW()
{
	SelectW();
}
