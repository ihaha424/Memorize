#pragma once
#include "D2DGameEngine/UIPanel.h"

class ElementsPanel : public UIPanel
{
public:
	ElementsPanel(World* _world);
	virtual ~ElementsPanel();

	class UIImage* q;
	class UIImage* w;
	class UIImage* e;
	class UIImage* r;

	std::vector<std::vector<UIImage*>> commands;

	void SelectFireSkill();
	void SelectWaterSkill();
	void SelectLightSkill();
	void SelectDarkSkill();

	void OnPressedQ();
	void OnPressedW();
};

