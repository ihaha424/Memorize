#include "framework.h"
#include "Level.h"
#include "Actor.h"
#include "World.h"
#include "CameraComponent.h"

Level::Level(const std::wstring& name)
{
	LevelName = name;
}

Level::~Level()
{
	for (auto& pGameObject : actorList)
	{
		delete pGameObject;
	}
	actorList.clear();
}

void Level::BeginPlay()
{
	for (auto actor : actorList)
	{
		actor->BeginPlay();
	}
}

void Level::FixedUpdate(float _fixedRate)
{
	for (auto actor : actorList)
	{
		if (actor->CheckTickProperty(TICK_PHYSICS))
		{
			actor->FixedUpdate(_fixedRate);
		}
	}
}

void Level::PreUpdate(float _dt)
{
	for (auto actor : actorList)
	{
		if (actor->CheckTickProperty(TICK_PRE_UPDATE))
		{
			actor->PreUpdate(_dt);
		}
	}
}

void Level::Update(float _dt)
{
	for (auto actor : actorList)
	{
		if (actor->CheckTickProperty(TICK_UPDATE))
		{
			actor->Update(_dt);
		}
	}
}

void Level::PostUpdate(float _dt)
{
	for (auto actor : actorList)
	{
		if (actor->CheckTickProperty(TICK_POST_UPDATE))
		{
			actor->PostUpdate(_dt);
		}
	}
}

void Level::Render(D2DRenderer* _renderer)
{
	for (auto actor : actorList)
	{
		if (actor->CheckTickProperty(TICK_RENDER))
		{
			if(GetWorld()->GetMainCamera()->InCameraArea(actor))
				actor->Render(_renderer);
		}
	}
}
