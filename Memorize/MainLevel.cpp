#include "MainLevel.h"
#include "MainLevelUIPanel.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Canvas.h"

MainLevel::MainLevel(World* _world, const std::wstring& _name)
	: Level(_world, _name)
{
	mainPanel = GetWorld()->GetCanvas()->CreatePannel<MainLevelUIPanel>(L"MainLevel");
}

MainLevel::~MainLevel()
{
}

void MainLevel::Enter()
{
}

void MainLevel::Exit()
{
}

void MainLevel::Clear()
{
}