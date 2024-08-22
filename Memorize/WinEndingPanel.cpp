#include "WinEndingPanel.h"
#include "D2DGameEngine/UIImage.h"
#include "D2DGameEngine/UIButton.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Canvas.h"
#include "D2DGameEngine/SoundManager.h"

WinEndingPanel::WinEndingPanel(World* _world)
	: UIPanel(_world)
{
	SetPosition(1920 / 2, 1080 / 2);

	CutSin[0] = CreateUI<UIImage>(L"cutSin1");
	CutSin[0]->SetPosition(0, 0);
	CutSin[0]->SetSprite(L"TestResource/WinEnding/Win_Endingscene01.png");
	CutSin[0]->SetZOrder(1);
	CutSin[0]->SetOpacity(0);
	CutSin[0]->Inactivate();

	CutSin[1] = CreateUI<UIImage>(L"cutSin2");
	CutSin[1]->SetPosition(0, 0);
	CutSin[1]->SetSprite(L"TestResource/WinEnding/Win_Endingscene02.png");
	CutSin[1]->SetZOrder(2);
	CutSin[1]->SetOpacity(0);
	CutSin[1]->Inactivate();


	CutSin[2] = CreateUI<UIImage>(L"cutSin3");
	CutSin[2]->SetPosition(0, 0);
	CutSin[2]->SetSprite(L"TestResource/WinEnding/Win_Endingscene03.png");
	CutSin[2]->SetZOrder(3);
	CutSin[2]->SetOpacity(0);
	CutSin[2]->Inactivate();


	Btn = CreateUI<UIButton>(L"StartButton");
	Btn->SetPosition(0, 0);
	Btn->AddOnClickHandler([this]() {
		if (ClickFlag)
		{
			if (index < 2)
			{
				index++;
				if (index == 2)
					SoundManager::PlayMusic(L"TestResource/Sound/Plus/locket_Close.wav.wav");
				ClickFlag = false;
				alphaTween->ResetTime();
			}
			else
			{
				mainMenu->Activate();
				mainMenuImage->Activate();
				Btn->Inactivate();
				alphaTween->ResetTime();
				btnActivate = true;
			}
		}
		SoundManager::PlayMusic(L"TestResource/Sound/Player/Skill/Sound_UI_Click.wav");
		}
	);
	Btn->SetSize(CutSin[0]->GetSize().x, CutSin[0]->GetSize().y);

	//MainMenu
	{
		mainMenu = CreateUI<UIButton>(L"mainMenu");
		mainMenu->SetPosition(0, 0);
		mainMenu->AddOnClickHandler([this]() {
			GetWorld()->SetNextScene(L"MainLevel");
			SoundManager::PlayMusic(L"TestResource/Sound/Player/Skill/Sound_UI_Click.wav");
			}
		);
		mainMenu->AddOnHoveredHandler([this]() { mainMenuImage->SetSprite(L"TestResource/WinEnding/Ending_w_the_end_mouseover.png"); });
		mainMenu->AddOnPressedHandler([this]() { mainMenuImage->SetSprite(L"TestResource/WinEnding/Ending_w_the_end_pushed.png"); });
		mainMenu->AddOnUnHoveredHandler([this]() { mainMenuImage->SetSprite(L"TestResource/WinEnding/Ending_w_the_end_normal.png"); });

		mainMenuImage = CreateUI<UIImage>(L"StartImage");
		mainMenuImage->SetSprite(L"TestResource/WinEnding/Ending_w_the_end_normal.png");
		mainMenuImage->SetPosition(0, 0);
		mainMenuImage->SetZOrder(4);
		mainMenuImage->SetOpacity(0);

		mainMenu->SetSize(mainMenuImage->GetSize().x, mainMenuImage->GetSize().y);
	}

	alphaTween = new DotTween<float>();
	alphaTween->SetData(&alpha);
	alphaTween->SetDuration(2.f);
	alphaTween->SetStartPoint(0);
	alphaTween->SetEndPoint(1);
	alphaTween->SetEasingEffect(EasingEffect::Linear);
	alphaTween->SetStepAnimation(StepAnimation::StepOnceForward);
}

WinEndingPanel::~WinEndingPanel()
{
	delete alphaTween;
}

void WinEndingPanel::BeginPlay()
{
	__super::BeginPlay();
	Inactivate();
	mainMenu->Inactivate();
	mainMenuImage->Inactivate();
}

void WinEndingPanel::Update(float _dt)
{
	__super::Update(_dt);
	if (!btnActivate)
	{
		if (!alphaTween->GetIsFinish())
		{
			alphaTween->Update(_dt);
			CutSin[index]->Activate();
			CutSin[index]->SetOpacity(alpha);
		}
		else
			ClickFlag = true;
	}
	else
	{
		alphaTween->Update(_dt);
		mainMenuImage->SetOpacity(alpha);
	}
}
