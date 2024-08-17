#include "PlayerMainUIPanel.h"
#include "D2DGameEngine/ProgressBar.h"

PlayerMainUIPanel::PlayerMainUIPanel(World* _world) : UIPanel(_world)
{
	SetPosition(1920 / 2, 950);
	hpBar = CreateUI<ProgressBar>(L"HPBar");
	mpBar = CreateUI<ProgressBar>(L"MPBar");


	hpBar->SetValue(1.0f);
	hpBar->SetDir(ProgressBarDir::LeftToRight);
	hpBar->SetFillImage(L"TestResource/Boss/BossHPBarFill.png");
	hpBar->SetPosition(0, -100);

	mpBar->SetValue(1.0f);
	mpBar->SetDir(ProgressBarDir::LeftToRight);
	mpBar->SetFillImage(L"TestResource/Boss/BossHPBarFill.png");
	mpBar->SetPosition(0,0);
}

PlayerMainUIPanel::~PlayerMainUIPanel()
{
}

void PlayerMainUIPanel::SetHPValue(float value)
{
	hpBar->SetValue(value);
}

void PlayerMainUIPanel::SetMPValue(float value)
{
	mpBar->SetValue(value);
}
