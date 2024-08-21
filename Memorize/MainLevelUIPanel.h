#pragma once
#include "D2DGameEngine/UIPanel.h"

class MainLevelUIPanel : public UIPanel
{
	class UIButton* startBtn;
	class UIImage* startImage;

	class UIButton* howToBtn;
	class UIImage* howToImage;
public:
	MainLevelUIPanel(class World* _world);
	virtual ~MainLevelUIPanel();

};

