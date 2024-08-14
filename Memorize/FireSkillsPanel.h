#pragma once
#include "D2DGameEngine/UIPanel.h"

class FireSkillsPanel : public UIPanel
{
public:
	FireSkillsPanel(World* _world);
	virtual ~FireSkillsPanel();

	class UIImage* q;
	class UIImage* w;
	class UIImage* e;
	class UIImage* r;

	std::vector<std::vector<UIImage*>> commands;

	void OnPressedQ();
	void OnPressedW();

};

