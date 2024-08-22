#pragma once
#include "D2DGameEngine/UIPanel.h"
class HowToPlayUIPanel : public UIPanel
{
	class UIImage* howtoplay;
	class UIButton* closeBtn;

public:
	HowToPlayUIPanel(World* _world);
	virtual ~HowToPlayUIPanel();

};

