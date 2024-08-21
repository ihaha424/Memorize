#include "MainLevel.h"
#include "MainLevelUIPanel.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Canvas.h"
#include "D2DGameEngine/Actor.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "CursorUIPanel.h"

#include "Loading.h"

#include "Cursor.h"

MainLevel::MainLevel(World* _world, const std::wstring& _name)
	: Level(_world, _name)
{}

MainLevel::~MainLevel()
{
}

void MainLevel::Enter()
{
	__super::Enter();
	GetWorld()->GetCanvas()->CreatePannel<CursorUIPanel>(L"Cursor");
	mainPanel = GetWorld()->GetCanvas()->CreatePannel<MainLevelUIPanel>(L"MainLevel");

	Actor* background = CreateActor<Actor>();
	AnimationBitmapComponent* backgroundBm = background->CreateComponent<AnimationBitmapComponent>();
	backgroundBm->SetSprite(L"TestResource/Main/01.png");
	backgroundBm->SliceSpriteSheet(1920, 1080, 0, 0, 0, 0);
	background->rootComponent = backgroundBm;
	background->SetTickProperties(TICK_RENDER);


	AnimationBitmapComponent* leftRocksBm = background->CreateComponent<AnimationBitmapComponent>();
	leftRocksBm->SetSprite(L"TestResource/Main/04.png");
	leftRocksBm->SliceSpriteSheet(322, 473, 0, 0, 0, 0);
	backgroundBm->AddChild(leftRocksBm);
	leftRocksBm->SetTranslation(-1920 / 2  + 35 + 322/2, -150 -35);

	AnimationBitmapComponent* characterBm = background->CreateComponent<AnimationBitmapComponent>();
	characterBm->SetSprite(L"TestResource/Main/02.png");
	characterBm->SliceSpriteSheet(850, 981, 0, 0, 0, 0);
	backgroundBm->AddChild(characterBm);
	characterBm->SetTranslation(-1920 / 2 + 425, 88);

	AnimationBitmapComponent* rightRocksBm = background->CreateComponent<AnimationBitmapComponent>();
	rightRocksBm->SetSprite(L"TestResource/Main/03.png");
	rightRocksBm->SliceSpriteSheet(677, 620, 0, 0, 0, 0);
	backgroundBm->AddChild(rightRocksBm);
	rightRocksBm->SetTranslation(1920/2 - 677 / 2 + 75, 1080/2 - 310);


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
