#include "PlayerMainUIPanel.h"
#include "D2DGameEngine/ProgressBar.h"

PlayerMainUIPanel::PlayerMainUIPanel(World* _world) : UIPanel(_world)
{
	SetPosition(1920 / 2, 800);
	hpBar = CreateUI<ProgressBar>(L"HPBar");
	mpBar = CreateUI<ProgressBar>(L"MPBar");

}

PlayerMainUIPanel::~PlayerMainUIPanel()
{
}
