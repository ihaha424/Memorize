#include "EnddingPanel.h"
#include "D2DGameEngine/UIImage.h"
#include "D2DGameEngine/UIButton.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Canvas.h"
#include "D2DGameEngine/SoundManager.h"

EnddingPanel::EnddingPanel(World* _world)
	: UIPanel(_world)
{
	SetPosition(1920 / 2, 1080 / 2);
	
	CutSin[0] = CreateUI<UIImage>(L"cutSin1");
	CutSin[0]->SetPosition(0, 0);
	CutSin[0]->SetSprite(L"TestResource/Ending/endingscene_01.png");
	CutSin[0]->SetZOrder(1);
	CutSin[0]->SetOpacity(0);
	CutSin[0]->Inactivate();

	CutSin[1] = CreateUI<UIImage>(L"cutSin2");
	CutSin[1]->SetPosition(0, 0);
	CutSin[1]->SetSprite(L"TestResource/Ending/endingscene_02.png");
	CutSin[1]->SetZOrder(2);
	CutSin[1]->SetOpacity(0);
	CutSin[1]->Inactivate();


	CutSin[2] = CreateUI<UIImage>(L"cutSin3");
	CutSin[2]->SetPosition(0, 0);
	CutSin[2]->SetSprite(L"TestResource/Ending/endingscene_03.png");
	CutSin[2]->SetZOrder(3);
	CutSin[2]->SetOpacity(0);
	CutSin[2]->Inactivate();


	Btn = CreateUI<UIButton>(L"StartButton");
	Btn->SetPosition(0,0);
	Btn->AddOnClickHandler([this]() {
		if (ClickFlag)
		{
			ClickFlag = false;
			alphaTween->ResetTime();
			if (index < 2)
				index++;
			else
			{
				reStartImage->Activate();
				reStart->Activate();
				mainMenu->Activate();
				mainMenuImage->Activate();
			}
		}
		SoundManager::PlayMusic(L"TestResource/Sound/Player/Skill/Sound_UI_Click.wav");
		}
	);
	Btn->SetSize(CutSin[0]->GetSize().x, CutSin[0]->GetSize().y);

	// ReStart
	{
		reStart = CreateUI<UIButton>(L"reStart");
		reStart->SetPosition(1920 / 2 + 470, 600);
		reStart->AddOnClickHandler([this]() {
			GetWorld()->SetNextScene(L"TestLevel");
			SoundManager::PlayMusic(L"TestResource/Sound/Player/Skill/Sound_UI_Click.wav");
			});
		reStart->AddOnHoveredHandler([this]() { reStartImage->SetSprite(L"TestResource/UI/button_play_2.png"); });
		reStart->AddOnPressedHandler([this]() { reStartImage->SetSprite(L"TestResource/UI/button_play_3.png"); });
		reStart->AddOnUnHoveredHandler([this]() { reStartImage->SetSprite(L"TestResource/UI/button_play_1.png"); });
		reStart->Inactivate();

		reStartImage = CreateUI<UIImage>(L"StartImage");
		reStartImage->SetSprite(L"TestResource/UI/button_play_1.png");
		reStartImage->SetPosition(1920 / 2 + 470, 600);
		reStartImage->SetZOrder(4);
		reStartImage->SetOpacity(0);
		reStartImage->Inactivate();

		reStart->SetSize(reStartImage->GetSize().x, reStartImage->GetSize().y);
	}

	//MainMenu
	{
		mainMenu = CreateUI<UIButton>(L"mainMenu");
		mainMenu->SetPosition(1920 / 2 + 470, 600);
		mainMenu->AddOnClickHandler([this]() {
			GetWorld()->SetNextScene(L"MainLevel"); 
			SoundManager::PlayMusic(L"TestResource/Sound/Player/Skill/Sound_UI_Click.wav"); 
			}
		);
		mainMenu->AddOnHoveredHandler([this]() { mainMenuImage->SetSprite(L"TestResource/UI/button_play_2.png"); });
		mainMenu->AddOnPressedHandler([this]() { mainMenuImage->SetSprite(L"TestResource/UI/button_play_3.png"); });
		mainMenu->AddOnUnHoveredHandler([this]() { mainMenuImage->SetSprite(L"TestResource/UI/button_play_1.png"); });
		mainMenu->Inactivate();

		mainMenuImage = CreateUI<UIImage>(L"StartImage");
		mainMenuImage->SetSprite(L"TestResource/UI/button_play_1.png");
		mainMenuImage->SetPosition(1920 / 2 + 470, 600);
		mainMenuImage->SetZOrder(4);
		mainMenuImage->SetOpacity(0);
		mainMenuImage->Inactivate();

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

EnddingPanel::~EnddingPanel()
{
	delete alphaTween;
}

void EnddingPanel::BeginPlay()
{
	__super::BeginPlay();
	Inactivate();
}

void EnddingPanel::Update(float _dt)
{
	__super::Update(_dt);
	if (!alphaTween->GetIsFinish())
	{
		CutSin[index]->Activate();
		CutSin[index]->SetOpacity(alpha);
		alphaTween->Update(_dt);
	}
	else
		ClickFlag = true;
}
