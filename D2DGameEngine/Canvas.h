#pragma once
#include "Actor.h"
#include "UIElement.h"

class Canvas : public Actor
{
	std::map<std::wstring, UIElement*> uiMap;
	std::list<class UIElement*> uiList;

public:
	Canvas(class World* _world);
	virtual ~Canvas();

	template<typename T>
	T* CreateUI(std::wstring name)
	{
		T* t = new T;
		UIElement* ui = dynamic_cast<UIElement*>(t);

		if (ui == nullptr)
			return nullptr;

		uiMap[name] = ui;
		uiList.push_back(ui);

		uiList.sort([](UIElement* u1, UIElement* u2)
			{return u1->GetZOrder() < u2->GetZOrder(); });

		return t;
	}

	template<typename T>
	T* GetUI(std::wstring name)
	{
		return dynamic_cast<T*>(uiMap[name]);
	}

	void ShowUI(std::wstring name)
	{
		uiMap[name]->SetIsActive(true);
	}
	void HideUI(std::wstring name)
	{
		uiMap[name]->SetIsActive(false);
	}

	void RemoveUI(std::wstring name)
	{
		UIElement* ui = uiMap[name];
		uiList.remove(ui);
		uiMap.erase(name);
		delete ui;
		ui = nullptr;
	}

	void AddToViewport();
	void RemoveFromViewport();

	virtual void Update(float _dt) override;
	virtual void Render(class D2DRenderer* _renderer) override;
};

