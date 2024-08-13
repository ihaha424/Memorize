#pragma once
#include "Actor.h"
#include "UIPanel.h"

class Canvas : public Actor
{
	std::map<std::wstring, UIPanel*> panelMap;
	std::vector<class UIPanel*> panelList;
public:
	Canvas(class World* _world);
	virtual ~Canvas();

	template<typename T>
	T* CreatePannel(std::wstring name)
	{
		T* t = new T;
		UIPanel* panel = dynamic_cast<UIPanel*>(t);

		if (panel == nullptr)
			return nullptr;

		panelMap[name] = panel;
		panelList.push_back(panel);

		std::sort(panelList.begin(), panelList.end(),
			[](UIPanel* panel, UIPanel* panel)
			{return panel->GetZOrder() < panel->GetZOrder(); });

		return t;
	}

	template<typename T>
	T* GetPanel(std::wstring name)
	{
		return dynamic_cast<T*>(panelMap[name]);
	}

	void ShowPanel(std::wstring name)
	{
		panelMap[name]->Activate();
	}
	void HidePanel(std::wstring name)
	{
		panelMap[name]->Inactivate();
	}

	void RemoveUI(std::wstring name)
	{
		UIPanel* panel = panelMap[name];
		remove(panelList.begin(), panelList.end(), panel);
		panelMap.erase(name);
		delete panel;
		panel = nullptr;
	}


	virtual void Update(float _dt) override;
	virtual void Render(class D2DRenderer* _renderer) override;
};

