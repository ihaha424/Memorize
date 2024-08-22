#include "MainLevel.h"
#include "MainLevelUIPanel.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Canvas.h"
#include "D2DGameEngine/Actor.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "CursorUIPanel.h"
#include "Loading.h"
#include "LeftRock.h"
#include "RightRock.h"


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

	Actor* background = CreateActor<Actor>();
	AnimationBitmapComponent* backgroundBm = background->CreateComponent<AnimationBitmapComponent>();
	backgroundBm->SetSprite(L"TestResource/Main/01.png");
	backgroundBm->SliceSpriteSheet(1920, 1080, 0, 0, 0, 0);
	background->rootComponent = backgroundBm;
	background->SetTickProperties(TICK_RENDER);
	background->renderLayer = -1;


	leftRock = CreateActor<LeftRock>();
	leftRock->SetLocation(-1920 / 2  + 200, -200);

	Actor* character = CreateActor<Actor>();
	AnimationBitmapComponent* characterBm = character->CreateComponent<AnimationBitmapComponent>();
	characterBm->SetSprite(L"TestResource/Main/02.png");
	characterBm->SliceSpriteSheet(850, 981, 0, 0, 0, 0);
	character->rootComponent = characterBm;
	characterBm->SetTranslation(-1920 / 2 + 425, 88);
	character->SetTickProperties(TICK_RENDER);
	character->renderLayer = 3;

	rightRock = CreateActor<RightRock>();
	rightRock->SetLocation(1920 / 2 - 677 / 2 + 75, 1080 / 2 - 310);

	__super::Enter();
}

void MainLevel::Exit()
{
	GetWorld()->GetCanvas()->HidePanel(L"MainLevel");
}

void MainLevel::Clear()
{
	for (auto& pGameObject : actorList)
	{
		delete pGameObject;
	}
	actorList.clear();
}
