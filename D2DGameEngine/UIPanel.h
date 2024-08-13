#pragma once
#include "UIElement.h"

class UIPanel :  public UIElement
{
	std::map<std::wstring, UIElement*> uiMap;
	std::list<class UIElement*> uiList;
	
public:
	UIPanel(class World* _world);
	virtual ~UIPanel();

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
		uiMap[name]->Activate();
	}
	void HideUI(std::wstring name)
	{
		uiMap[name]->Inactivate();
	}

	void RemoveUI(std::wstring name)
	{
		UIElement* ui = uiMap[name];
		uiList.remove(ui);
		uiMap.erase(name);
		delete ui;
		ui = nullptr;
	}

	virtual void Update(float _dt) override;
	virtual void Render(class D2DRenderer* _renderer) override;
};

