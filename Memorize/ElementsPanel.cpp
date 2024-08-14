#include "ElementsPanel.h"
#include "FireSkillsPanel.h"
#include "D2DGameEngine/UIImage.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Canvas.h"

ElementsPanel::ElementsPanel(World* _world) : UIPanel(_world)
{
	SetPosition(70, 240);

	q = CreateUI<UIImage>(L"Q");
	q->SetSprite(L"TestResource/UI/Q.png");
	q->SetPosition(0, -90);

	w = CreateUI<UIImage>(L"W");
	w->SetSprite(L"TestResource/UI/W.png");
	w->SetPosition(0, -30);

	e = CreateUI<UIImage>(L"E");
	e->SetSprite(L"TestResource/UI/E.png");
	e->SetPosition(0, 30);

	r = CreateUI<UIImage>(L"R");
	r->SetSprite(L"TestResource/UI/R.png");
	r->SetPosition(0, 90);


	GetWorld()->GetCanvas()->CreatePannel<FireSkillsPanel>(L"FireSkills");
	GetWorld()->GetCanvas()->HidePanel(L"FireSkills");
}

ElementsPanel::~ElementsPanel()
{
}


void ElementsPanel::OnPressedQ()
{
	if (status == OS_INACTIVE) return;

	GetWorld()->GetCanvas()->HidePanel(L"Elements");
	GetWorld()->GetCanvas()->ShowPanel(L"FireSkills");
}
void ElementsPanel::OnPressedW()
{
	if (status == OS_INACTIVE) return;

}
