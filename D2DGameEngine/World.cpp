#include "framework.h"
#include "World.h"
#include "Level.h"

World::World() {}
World::~World()
{
	auto iter = vLevelList.begin();
	for (; iter != vLevelList.end(); iter++)
	{
		delete iter->second;
	}
	vLevelList.clear();
}

void World::ChangeScene()
{
	if (NextLevel)
	{
		CurLevel->Exit();
		CurLevel->Clear();
		CurLevel = NextLevel;
		NextLevel = nullptr;
		CurLevel->Enter();
	}
}

void World::SetNextScene(std::wstring nextLevel)
{
	const auto iter = vLevelList.find(nextLevel);
	if (iter != vLevelList.end())
	{
		assert(iter != vLevelList.end());
	}
	NextLevel = iter->second;
}

void World::Update(const float& deltaTime)
{
	CurLevel->Update(deltaTime);
}

void World::Render(ID2D1HwndRenderTarget* pRenderTarget)
{
	CurLevel->Render(pRenderTarget);
}
void World::LateUpdate(const float& deltaTime)
{

	CurLevel->LateUpdate(deltaTime);
}
void World::PhysicalUpdate()
{
	CurLevel->PhysicalUpdate();
}