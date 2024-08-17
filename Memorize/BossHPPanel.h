#pragma once
#include "D2DGameEngine/UIPanel.h"

class BossHPPanel : public UIPanel
{
	class ProgressBar* hpBar;

public:
	BossHPPanel(World* _world);
	virtual ~BossHPPanel();

	
	void SetValue(float _value);

};

