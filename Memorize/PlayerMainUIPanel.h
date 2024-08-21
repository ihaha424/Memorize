#pragma once
#include "D2DGameEngine/UIPanel.h"
class PlayerMainUIPanel : public UIPanel
{

	class ProgressBar* hpBar;
	class ProgressBar* mpBar;

	class UIImage* back;
	class UIImage* front;

	class UIImage* memorize;
	class UIImage* flash;

public:
	PlayerMainUIPanel(class World* _world);
	virtual ~PlayerMainUIPanel();

	void SetHPValue(float value);
	void SetMPValue(float value);
	void SetMemorizeOn();
	void SetMemorizeOff();
	void SetFlashOn();
	void SetFlashOff();
};

