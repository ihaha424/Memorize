#pragma once
#include "D2DGameEngine/UIPanel.h"
class PlayerMainUIPanel : public UIPanel
{

	class ProgressBar* hpBar;
	class ProgressBar* mpBar;

	class UIImage* back;
	class UIImage* front;

public:
	PlayerMainUIPanel(class World* _world);
	virtual ~PlayerMainUIPanel();

	void SetHPValue(float value);
	void SetMPValue(float value);
};

