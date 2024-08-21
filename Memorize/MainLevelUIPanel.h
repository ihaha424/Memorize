#pragma once
#include "D2DGameEngine/UIPanel.h"

class MainLevelUIPanel : public UIPanel
{
	class UIButton* startBtn;
	class UIImage* startImage;
public:
	MainLevelUIPanel(class World* _world);
	virtual ~MainLevelUIPanel();

};

