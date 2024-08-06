#include "framework.h"
#include "World.h"
#include "Level.h"

World::World() {}
World::~World()
{
	auto iter = levelList.begin();
	for (; iter != levelList.end(); iter++)
	{
		delete iter->second;
	}
	levelList.clear();
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
	const auto iter = levelList.find(nextLevel);
	if (iter != levelList.end())
	{
		assert(iter != levelList.end());
	}
	NextLevel = iter->second;
}

void World::AddUICanvas(Canvas* canvas)
{
	activeUICanvasList.push_back(canvas);
}

void World::RemoveUICanvas(Canvas* canvas)
{
	remove(activeUICanvasList.begin(), activeUICanvasList.end(), canvas);
}

void World::FixedUpdate(float _fixedRate)
{
	CurLevel->FixedUpdate(_fixedRate);
}

void World::PreUpdate(float _dt)
{
	CurLevel->PreUpdate(_dt);
}

void World::Update(float _dt)
{
	CurLevel->Update(_dt);
}

void World::PostUpdate(float _dt)
{
	CurLevel->PostUpdate(_dt);
}

void World::Render(D2DRenderer* _renderer)
{
	CurLevel->Render(_renderer);
}