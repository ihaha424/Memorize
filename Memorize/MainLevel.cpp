#include "MainLevel.h"
#include "MainLevelUIPanel.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Canvas.h"
#include "D2DGameEngine/Actor.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "CursorUIPanel.h"
#include "Loading.h"
#include "LeftRock.h"
#include "Rock01.h"
#include "Rock03.h"
#include "Rock04.h"
#include "Rock05.h"
#include "Title.h"
#include "RightRock.h"
#include "HowToPlayUIPanel.h"
#include "D2DGameEngine/UIButton.h"


MainLevel::MainLevel(World* _world, const std::wstring& _name)
	: Level(_world, _name)
{}

MainLevel::~MainLevel()
{
}

void MainLevel::Enter()
{
	SoundManager::PlayMusic(L"TestResource/Sound/BG/Sound_MainBGM.wav", 0);

	GetWorld()->GetCanvas()->CreatePannel<CursorUIPanel>(L"Cursor");
	mainPanel = GetWorld()->GetCanvas()->CreatePannel<MainLevelUIPanel>(L"MainLevel");
	howtoplayPanel = GetWorld()->GetCanvas()->CreatePannel<HowToPlayUIPanel>(L"HowToPlay");
	GetWorld()->GetCanvas()->HidePanel(L"HowToPlay");

	Actor* background = CreateActor<Actor>();
	AnimationBitmapComponent* backgroundBm = background->CreateComponent<AnimationBitmapComponent>();
	backgroundBm->SetSprite(L"TestResource/Main/01.png");
	backgroundBm->SliceSpriteSheet(1920, 1080, 0, 0, 0, 0);
	background->rootComponent = backgroundBm;
	background->SetTickProperties(TICK_RENDER);
	background->renderLayer = -1;

	leftRock = CreateActor<LeftRock>();
	leftRock->SetLocation(-1920 / 2  + 200, -200);

	Rock01* leftRock01 = CreateActor<Rock01>();
	leftRock01->SetLocation(-SCREEN_WIDTH / 2 + 440, -SCREEN_HEIGHT / 2 + 80);

	Rock03* leftRock03 = CreateActor<Rock03>();
	leftRock03->SetLocation(0, SCREEN_HEIGHT / 2 - 50);

	Rock04* leftRock04 = CreateActor<Rock04>();
	leftRock04->SetLocation(-SCREEN_WIDTH / 2 + 50, 240);

	Rock05* leftRock05 = CreateActor<Rock05>();
	leftRock05->SetLocation(SCREEN_WIDTH / 2 - 80, -100);

	rightRock = CreateActor<RightRock>();
	rightRock->SetLocation(1920 / 2 - 677 / 2 + 75, 1080 / 2 - 310);

	Title* title = CreateActor<Title>();
	title->SetLocation(400, -200);
	title->renderLayer = 3;
	__super::Enter();
}

void MainLevel::Exit()
{
	GetWorld()->GetCanvas()->HidePanel(L"MainLevel");
	__super::Exit();
}

void MainLevel::Clear()
{
	for (auto& pGameObject : actorList)
	{
		delete pGameObject;
	}
	actorList.clear();
}
