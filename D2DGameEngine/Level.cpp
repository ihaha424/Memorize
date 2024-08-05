#include "framework.h"
#include "Level.h"

Level::Level(const std::wstring& name)
{
	LevelName = name;
}

Level::~Level()
{
	for (auto& pGameObject : vGameObjectList)
	{
		delete pGameObject;
	}
	vGameObjectList.clear();
}

void Level::Update(const float& deltaTime)
{
	for (auto objList : vGameObjectList)
	{
		//objList->SuperUpdate(deltaTime);
	}
}

void Level::LateUpdate(const float& deltaTime)
{
	for (auto objList : vGameObjectList)
	{
		//objList->SuperLateUpdate(deltaTime);
	}

	////MouseUpdate...
	//MouseComponent::GetInstance()->Update(deltaTime);
}

void Level::PhysicalUpdate()
{
	//colliderManager.Update(vGameObjectList);
}

void Level::Render(ID2D1HwndRenderTarget* pRenderTarget)
{
	for (auto objList : vGameObjectList)
	{
		//objList->SuperRender(pRenderTarget);
	}

	//
	//	UI Update
	//

	// MainMouse -> 음 마우스가 다른애의 컴포먼트로 들어갈 경우는 어카지?
	// Update는 또 안하나?
	// Level에서 Mouse의 부모 오브젝트를 들고 있어야 하나?
	//MouseComponent::GetInstance()->Render(pRenderTarget);
}