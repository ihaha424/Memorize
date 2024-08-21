#include "MainLevel.h"
#include "MainLevelUIPanel.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Canvas.h"

#include "Loading.h"

MainLevel::MainLevel(World* _world, const std::wstring& _name)
	: Level(_world, _name)
{}

MainLevel::~MainLevel()
{
}

void MainLevel::Enter()
{
	__super::Enter();
	mainPanel = GetWorld()->GetCanvas()->CreatePannel<MainLevelUIPanel>(L"MainLevel");
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
