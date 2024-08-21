#pragma once
#include "D2DGameEngine/UIPanel.h"
class PlayerMainUIPanel : public UIPanel
{

	class ProgressBar* hpBar;
	class ProgressBar* mpBar;

	class UIText* hpText;
	class UIText* mpText;

	class UIImage* back;
	class UIImage* front;

	class UIImage* memorize;
	class UIImage* flash;

public:
	PlayerMainUIPanel(class World* _world);
	virtual ~PlayerMainUIPanel();

	void SetMemorizeOn();
	void SetMemorizeOff();
	void SetFlashOn();
	void SetFlashOff();

	void SetHPInfo(float hp, float maxHp);
	void SetMPInfo(float mp, float maxMp);
};

