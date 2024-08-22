#pragma once
#include "D2DGameEngine/UIPanel.h"

class ElementsPanel : public UIPanel
{
public:
	ElementsPanel(World* _world);
	virtual ~ElementsPanel();

	class UIImage* ring;

	class UIImage* q;
	class UIImage* w;
	class UIImage* e;
	class UIImage* r;

	class UIButton* qBtn;
	class UIButton* wBtn;
	class UIButton* eBtn;
	class UIButton* rBtn;

	std::vector<class UIText*> infoTexts;
	std::vector<class UIImage*> textBGs;

	class ID2D1Bitmap* Qicon;
	class ID2D1Bitmap* Wicon;
	class ID2D1Bitmap* Eicon;
	class ID2D1Bitmap* Ricon;

	class ID2D1Bitmap* Qbm;
	class ID2D1Bitmap* Wbm;
	class ID2D1Bitmap* Ebm;
	class ID2D1Bitmap* Rbm;

	class ID2D1Bitmap* Qbm_off;
	class ID2D1Bitmap* Wbm_off;
	class ID2D1Bitmap* Ebm_off;
	class ID2D1Bitmap* Rbm_off;

	std::vector<std::vector<int>> fireCommands;
	std::vector<std::vector<int>> waterCommands;
	std::vector<std::vector<int>> lightCommands;
	std::vector<std::vector<int>> darkCommands;

	std::vector<std::vector<class UIImage*>> commands;

	class GPlayerController* playerController;

	bool ending = false;
	float elapsedTime = 0.f;

	virtual void Update(float _dt) override;
	void SetPlayerController(class GPlayerController* _playerController) { playerController = _playerController; }

	void SetQWER(std::vector<std::vector<int>> elementCommands);
	void SetQWER(std::vector<std::vector<int>> elementCommands, int type);
	void SetOff(std::vector<std::vector<int>> elementCommands, int type, int index);
	void HideAllCommands();
	void SetSkillList();
	void ShowSkillInfo(enum ESkillType curSkillType);
	void HideSkillInfo(enum ESkillType curSkillType);
	std::vector<std::vector<int>> CheckSkillType();
};

