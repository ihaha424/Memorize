#include "MainLevelUIPanel.h"
#include "TestLevel.h"
#include "CursorUIPanel.h"
#include "D2DGameEngine/UIImage.h"
#include "D2DGameEngine/UIButton.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Canvas.h"

MainLevelUIPanel::MainLevelUIPanel(World* _world) : UIPanel(_world)
{
	startBtn = CreateUI<UIButton>(L"StartButton");
	startBtn->SetPosition(1920 / 2 + 300, 600);
	startBtn->AddOnClickHandler([this]() {GetWorld()->SetNextScene(L"TestLevel"); });
	startBtn->AddOnHoveredHandler([this]() { startImage->SetSprite(L"TestResource/UI/button_play_2.png"); });
	startBtn->AddOnPressedHandler([this]() { startImage->SetSprite(L"TestResource/UI/button_play_3.png"); });
	startBtn->AddOnUnHoveredHandler([this]() { startImage->SetSprite(L"TestResource/UI/button_play_1.png"); });

	CursorUIPanel* cursor = GetWorld()->GetCanvas()->GetPanel<CursorUIPanel>(L"Cursor");
	startBtn->AddOnHoveredHandler([=]() { cursor->SetCursorImage(L"TestResource/Cursors/Cursor_Pointer.png"); });
	startBtn->AddOnPressedHandler([=]() { cursor->SetCursorImage(L"TestResource/Cursors/Cursor_Click.png"); });
	startBtn->AddOnUnHoveredHandler([=]() { cursor->SetCursorImage(L"TestResource/Cursors/Cursor_Default.png"); });

	startImage = CreateUI<UIImage>(L"StartImage");
	startImage->SetSprite(L"TestResource/UI/button_play_1.png");
	startImage->SetPosition(1920 / 2 + 300, 600);
	startImage->SetZOrder(1);
	startBtn->SetSize(startImage->GetSize().x, startImage->GetSize().y);

}

MainLevelUIPanel::~MainLevelUIPanel()
{
}
