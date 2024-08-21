#include "BossHPPanel.h"
#include "Boss.h"
#include "GPlayerController.h"
#include "D2DGameEngine/ProgressBar.h"
#include "D2DGameEngine/UIImage.h"


BossHPPanel::BossHPPanel(World* _world) : UIPanel(_world)
{
	SetPosition(1920 / 2, 55);
	back = CreateUI<UIImage>(L"BossUIBack");
	back->SetSprite(L"TestResource/UI/Boss_HP.png");
	back->SetPosition(0, 0);
	back->SetZOrder(0);

	hpBar = CreateUI<ProgressBar>(L"BossHP");
	hpBar->SetValue(1.0f);
	hpBar->SetDir(ProgressBarDir::LeftToRight);
	hpBar->SetFillImage(L"TestResource/UI/Boss_CHP.png");
	hpBar->SetPosition(0, 0);
	hpBar->SetZOrder(1);
}

BossHPPanel::~BossHPPanel()
{
}


void BossHPPanel::SetValue(float _value)
{
	hpBar->SetValue(_value);
}


