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

	std::vector<class UIText*> infoTexts;

	class ID2D1Bitmap* Qbm;
	class ID2D1Bitmap* Wbm;
	class ID2D1Bitmap* Ebm;
	class ID2D1Bitmap* Rbm;

	std::vector<std::vector<int>> fireCommands;
	std::vector<std::vector<int>> waterCommands;
	std::vector<std::vector<int>> lightCommands;
	std::vector<std::vector<int>> darkCommands;

	std::vector<std::vector<class UIImage*>> commands;

	class GPlayerController* playerController;

	virtual void Update(float _dt) override;
	void SetPlayerController(class GPlayerController* _playerController) { playerController = _playerController; }

	void SetQWER(std::vector<std::vector<int>>& elementCommands);
	void SetQWER(std::vector<std::vector<int>>& elementCommands, int type);
	void HideAllCommands();
	void SetSkillList();
	std::vector<std::vector<int>>& CheckSkillType();
};

