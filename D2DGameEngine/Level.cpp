#include "framework.h"
#include "Level.h"
#include "Actor.h"
#include "World.h"
#include "CameraComponent.h"
#include "CameraActor.h"
#include "D2DRenderer.h"

Level::Level(class World* _world, const std::wstring& _name)
{
	world = _world;
	LevelName = _name;
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
	//메인카메라가 비어있으면 카메라액터를 생성하여 추가 
	if (GetWorld()->GetMainCamera() == nullptr)
	{
		CameraActor* cameraActor = CreateActor<CameraActor>();
		GetWorld()->SetMainCamera(cameraActor->GetComponent<CameraComponent>());
	}

	for (auto actor : actorList)
	{
		if(actor->GetStatus() == OS_AWAKE)
			actor->BeginPlay();
	}
}

void Level::FixedUpdate(float _fixedRate)
{
	for (auto actor : actorList)
	{
		if (actor->CheckTickProperty(TICK_PHYSICS) && actor->GetStatus() == OS_ACTIVE)
		{
			actor->FixedUpdate(_fixedRate);
		}
	}
}

void Level::PreUpdate(float _dt)
{
	for (auto actor : actorList)
	{
		if (actor->CheckTickProperty(TICK_PRE_UPDATE) && actor->GetStatus() == OS_ACTIVE)
		{
			actor->PreUpdate(_dt);
		}
	}
}

void Level::Update(float _dt)
{
	for (auto actor : actorList)
	{
		if (actor->CheckTickProperty(TICK_UPDATE) && actor->GetStatus() == OS_ACTIVE)
		{
			actor->Update(_dt);
		}
	}
}

void Level::PostUpdate(float _dt)
{
	for (auto actor : actorList)
	{
		if (actor->CheckTickProperty(TICK_POST_UPDATE) && actor->GetStatus() == OS_ACTIVE)
		{
			actor->PostUpdate(_dt);
		}
	}
	
	auto it = std::_Erase_remove_if(actorList,
		[=](Actor* actor) {
			if (actor->GetStatus() == OS_DESTROY)
			{
				delete actor;
				return 1;
			}
			return 0;
		});
}

void Level::Render(D2DRenderer* _renderer)
{
	Math::Matrix cameraTF = GetWorld()->GetMainCamera()->GetWorldTransform();
	cameraTF = cameraTF * Math::Matrix::CreateTranslation(-CameraComponent::screenSize.x / 2, -CameraComponent::screenSize.y / 2, 0.f);
	_renderer->PushTransform(cameraTF.Invert());

	for (auto actor : actorList)
	{
		if (actor->CheckTickProperty(TICK_RENDER) && actor->GetStatus() == OS_ACTIVE)
		{
			if(GetWorld()->GetMainCamera()->InCameraArea(actor))
				actor->Render(_renderer);
		}
	}
	_renderer->PopTransform();
}
