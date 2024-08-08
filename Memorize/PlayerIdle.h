#pragma once

#include "PlayerState.h"

class PlayerIdle
	:public PlayerState
{
	// PlayerState을(를) 통해 상속됨
	void Enter() override;
	void Update(float _dt) override;
	void Exit() override;

	// PlayerState을(를) 통해 상속됨
	void Q() override;
	void W() override;
	void E() override;
	void R() override;
	void Left() override;
	void Right() override;
};

