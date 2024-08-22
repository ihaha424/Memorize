#pragma once
#include "../D2DGameEngine/Level.h"

class Practice : public Level
{
	using Super = Level;

	class Player* player;
	class Scarecrow* boss;
	class GPlayerController* playerController;

	class ElementsPanel* elementsPanel;

	bool statusRegen{ false };


	float skillTimer{ 5.f };

public:
	Practice(class World* _world, const std::wstring& _name);
	virtual ~Practice();


	class BossHPPanel* bossHpBar = nullptr;
	class DisfellPanel* disfellPanel = nullptr;
	class PlayerMainUIPanel* playerMainUI = nullptr;
	virtual void Enter() override;
	virtual void Exit() { __super::Exit(); };
	virtual void Clear();

	virtual void Update(float _dt) override;
};

