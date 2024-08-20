#include "MainLevelUIPanel.h"
#include "TestLevel.h"
#include "D2DGameEngine/UIImage.h"
#include "D2DGameEngine/UIButton.h"
#include "D2DGameEngine/World.h"

MainLevelUIPanel::MainLevelUIPanel(World* _world) : UIPanel(_world)
{
	startBtn = CreateUI<UIButton>(L"StartButton");
	startBtn->SetPosition(1920 / 2, 800);
	startBtn->SetSize(200, 100);
	startBtn->AddOnClickHandler([this]() {GetWorld()->SetNextScene(L"TestLevel"); });

	startImage = CreateUI<UIImage>(L"StartImage");
	startImage->SetSprite(L"TestResource/UI/ElementSkillUI.png");
	startImage->SetPosition(1920 / 2, 800);
	startBtn->SetSize(200, 100);
}

MainLevelUIPanel::~MainLevelUIPanel()
{
}
