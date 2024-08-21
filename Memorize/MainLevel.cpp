#include "MainLevel.h"
#include "MainLevelUIPanel.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Canvas.h"
#include "D2DGameEngine/Actor.h"
#include "D2DGameEngine/BitmapComponent.h"
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
	BitmapComponent* backgroundBm = background->CreateComponent<BitmapComponent>();
	backgroundBm->SetSprite(L"TestResource/MainImage.png");
	background->rootComponent = backgroundBm;
	background->SetTickProperties(TICK_RENDER);

	
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
