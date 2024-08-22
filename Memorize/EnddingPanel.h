#pragma once
#include "D2DGameEngine/UIPanel.h"
#include "D2DGameEngine/DotTween.h"

class EnddingPanel : public UIPanel
{
	class UIImage* CutSin[3];

	class UIButton* Btn;

	class UIButton* reStart;
	class UIImage* reStartImage;
	class UIButton* mainMenu;
	class UIImage* mainMenuImage;
public:
	EnddingPanel(World* _world);
	virtual ~EnddingPanel();


	virtual void BeginPlay() override;
	virtual void Update(float _dt) override;

	

private:
	float alpha;
	DotTween<float>* alphaTween;
	int index{0};
	bool ClickFlag{ true };
};

