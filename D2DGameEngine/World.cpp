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