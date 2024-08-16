#include "BossHPPanel.h"
#include "D2DGameEngine/ProgressBar.h"

BossHPPanel::BossHPPanel(World* _world) : UIPanel(_world)
{
	hpBar = CreateUI<ProgressBar>(L"BossHP");
	hpBar->SetValue(1.0f);
	hpBar->SetPosition(1920/2, 30);
	hpBar->SetDir(ProgressBarDir::LeftToRight);
	hpBar->SetFillImage(L"TestResource/Boss/BossHPBarFill.png");
}

BossHPPanel::~BossHPPanel()
{
}

void BossHPPanel::SetValue(float _value)
{
	hpBar->SetValue(_value);
}

