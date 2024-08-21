#include "PlayerMainUIPanel.h"
#include "D2DGameEngine/ProgressBar.h"
#include "D2DGameEngine/UIImage.h"

PlayerMainUIPanel::PlayerMainUIPanel(World* _world) : UIPanel(_world)
{
	SetPosition(1920 / 2, 950);
	hpBar = CreateUI<ProgressBar>(L"HPBar");
	mpBar = CreateUI<ProgressBar>(L"MPBar");
	back = CreateUI<UIImage>(L"PlayerUIBack");
	back->SetSprite(L"TestResource/UI/UI_middle_01.png");
	back->SetPosition(0, -42);
	back->SetZOrder(0);

	front = CreateUI<UIImage>(L"PlayerUIFront");
	front->SetSprite(L"TestResource/UI/UI_middle_02.png");
	front->SetPosition(0, 0);
	front->SetZOrder(2);

	hpBar->SetValue(1.0f);
	hpBar->SetDir(ProgressBarDir::RightToLeft);
	hpBar->SetFillImage(L"TestResource/UI/UI_middle_status_HP.png");
	hpBar->SetPosition(-150, -33);
	hpBar->SetZOrder(1);

	mpBar->SetValue(1.0f);
	mpBar->SetDir(ProgressBarDir::LeftToRight);
	mpBar->SetFillImage(L"TestResource/UI/UI_middle_status_MP.png");
	mpBar->SetPosition(150, -33);
	mpBar->SetZOrder(1);

	memorize = CreateUI<UIImage>(L"PlayerUIMemorize");
	memorize->SetSprite(L"TestResource/UI/Icon_Memorize_off.png");
	memorize->SetPosition(-243, 21);
	memorize->SetZOrder(3);

	flash = CreateUI<UIImage>(L"PlayerUIMemorize");
	flash->SetSprite(L"TestResource/UI/Icon_Flash_off.png");
	flash->SetPosition(243, 21);
	flash->SetZOrder(3);
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

void PlayerMainUIPanel::SetMemorizeOn()
{
	memorize->SetSprite(L"TestResource/UI/Icon_Memorize.png");
}

void PlayerMainUIPanel::SetMemorizeOff()
{
	memorize->SetSprite(L"TestResource/UI/Icon_Memorize_off.png");
}

void PlayerMainUIPanel::SetFlashOn()
{
	flash->SetSprite(L"TestResource/UI/Icon_Flash.png");
}

void PlayerMainUIPanel::SetFlashOff()
{
	flash->SetSprite(L"TestResource/UI/Icon_Flash_off.png");
}
