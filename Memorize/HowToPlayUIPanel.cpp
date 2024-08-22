#include "HowToPlayUIPanel.h"
#include "D2DGameEngine/UIButton.h"
#include "D2DGameEngine/UIImage.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Canvas.h"
#include "CursorUIPanel.h"
#include "MainLevelUIPanel.h"

HowToPlayUIPanel::HowToPlayUIPanel(World* _world) : UIPanel(_world)
{
	SetPosition(1920 / 2, 1080 / 2);
	howtoplay = CreateUI<UIImage>(L"HowToPlayImage");
	howtoplay->SetSprite(L"TestResource/Main/howtoplay.png");
	howtoplay->SetPosition(0, 0);

	closeBtn = CreateUI<UIButton>(L"HowToPlayCloseButton");
	closeBtn->SetSize(88, 77);
	closeBtn->SetPosition(1920 / 2 - 188, -1080 / 2 + 120);
	closeBtn->AddOnClickHandler([this]() {GetWorld()->GetCanvas()->HidePanel(L"HowToPlay"); });
	closeBtn->AddOnClickHandler([this]() {GetWorld()->GetCanvas()->GetPanel<MainLevelUIPanel>(L"MainLevel")->ShowUI(L"StartButton"); });
	closeBtn->AddOnClickHandler([this]() {GetWorld()->GetCanvas()->GetPanel<MainLevelUIPanel>(L"MainLevel")->ShowUI(L"HowToButton"); });

	CursorUIPanel* cursor = GetWorld()->GetCanvas()->GetPanel<CursorUIPanel>(L"Cursor");
	closeBtn->AddOnHoveredHandler([=]() { cursor->SetCursorImage(L"TestResource/Cursors/Cursor_Pointer.png"); });
	closeBtn->AddOnPressedHandler([=]() { cursor->SetCursorImage(L"TestResource/Cursors/Cursor_Click.png"); });
	closeBtn->AddOnUnHoveredHandler([=]() { cursor->SetCursorImage(L"TestResource/Cursors/Cursor_Default.png"); });
}

HowToPlayUIPanel::~HowToPlayUIPanel()
{
}
