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
	CutSin[0]->SetSprite(L"TestResource/Ending/Endingscene01.png");
	CutSin[0]->SetZOrder(1);
	CutSin[0]->SetOpacity(0);
	CutSin[0]->Inactivate();

	CutSin[1] = CreateUI<UIImage>(L"cutSin2");
	CutSin[1]->SetPosition(0, 0);
	CutSin[1]->SetSprite(L"TestResource/Ending/Endingscene02.png");
	CutSin[1]->SetZOrder(2);
	CutSin[1]->SetOpacity(0);
	CutSin[1]->Inactivate();


	CutSin[2] = CreateUI<UIImage>(L"cutSin3");
	CutSin[2]->SetPosition(0, 0);
	CutSin[2]->SetSprite(L"TestResource/Ending/Endingscene03.png");
	CutSin[2]->SetZOrder(3);
	CutSin[2]->SetOpacity(0);
	CutSin[2]->Inactivate();


	Btn = CreateUI<UIButton>(L"StartButton");
	Btn->SetPosition(0,0);
	Btn->AddOnClickHandler([this]() {
		if (ClickFlag)
		{
			if (index < 2)
			{
				index++;
				if(index == 3)
					SoundManager::PlayMusic(L"TestResource/Sound/Plus/locket_Open.wav.wav");
				ClickFlag = false;
				alphaTween->ResetTime();
			}
			else
			{
				reStartImage->Activate();
				reStart->Activate();
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

	// ReStart
	{
		reStart = CreateUI<UIButton>(L"reStart");
		reStart->SetPosition(-300,0);
		reStart->AddOnClickHandler([this]() {
			GetWorld()->SetNextScene(L"TestLevel");
			SoundManager::PlayMusic(L"TestResource/Sound/Player/Skill/Sound_UI_Click.wav");
			});
		reStart->AddOnHoveredHandler([this]() { reStartImage->SetSprite(L"TestResource/UI/button_play_2.png"); });
		reStart->AddOnPressedHandler([this]() { reStartImage->SetSprite(L"TestResource/UI/button_play_3.png"); });
		reStart->AddOnUnHoveredHandler([this]() { reStartImage->SetSprite(L"TestResource/UI/button_play_1.png"); });

		reStartImage = CreateUI<UIImage>(L"StartImage");
		reStartImage->SetSprite(L"TestResource/UI/button_play_1.png");
		reStartImage->SetPosition(-300, 0);
		reStartImage->SetZOrder(4);
		reStartImage->SetOpacity(0);

		reStart->SetSize(reStartImage->GetSize().x, reStartImage->GetSize().y);
	}

	//MainMenu
	{
		mainMenu = CreateUI<UIButton>(L"mainMenu");
		mainMenu->SetPosition(300,0);
		mainMenu->AddOnClickHandler([this]() {
			GetWorld()->SetNextScene(L"MainLevel"); 
			SoundManager::PlayMusic(L"TestResource/Sound/Player/Skill/Sound_UI_Click.wav"); 
			}
		);
		mainMenu->AddOnHoveredHandler([this]() { mainMenuImage->SetSprite(L"TestResource/Ending/Ending_d_giveup_mouseover.png"); });
		mainMenu->AddOnPressedHandler([this]() { mainMenuImage->SetSprite(L"TestResource/Ending/Ending_d_giveup_pushed.png"); });
		mainMenu->AddOnUnHoveredHandler([this]() { mainMenuImage->SetSprite(L"TestResource/Ending/Ending_d_giveup_normal.png"); });

		mainMenuImage = CreateUI<UIImage>(L"StartImage");
		mainMenuImage->SetSprite(L"TestResource/Ending/Ending_d_giveup_normal.png");
		mainMenuImage->SetPosition(300, 0);
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

EnddingPanel::~EnddingPanel()
{
	delete alphaTween;
}

void EnddingPanel::BeginPlay()
{
	__super::BeginPlay();
	Inactivate();
	mainMenu->Inactivate();
	mainMenuImage->Inactivate();
	reStartImage->Inactivate();
	reStart->Inactivate();
}

void EnddingPanel::Update(float _dt)
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
		reStartImage->SetOpacity(alpha);
		mainMenuImage->SetOpacity(alpha);
	}
}
