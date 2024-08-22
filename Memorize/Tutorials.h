#pragma once
#include "../D2DGameEngine/Level.h"

#include "TutorialGuide.h"

class Tutorials : public Level
{
	using Super = Level;

	class TutorialPlayer* player;
	class GPlayerController* playerController;

	class ElementsPanel* elementsPanel;

	TutorialGuide tutorialGuide;

	bool statusRegen{ false };

public:
    Tutorials(class World* _world, const std::wstring& _name);
    virtual ~Tutorials();


    class BossHPPanel* bossHpBar = nullptr;
    class DisfellPanel* disfellPanel = nullptr;
    class PlayerMainUIPanel* playerMainUI = nullptr;
    virtual void Enter() override;
    virtual void Exit() { __super::Exit(); };
    virtual void Clear();

		virtual void Update(float _dt) override;

private:

	void SetupTutorialGuide();
};

