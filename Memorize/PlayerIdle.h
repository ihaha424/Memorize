#pragma once

#include "PlayerState.h"

class PlayerIdle
	:public PlayerState
{
public:
	PlayerIdle(FSMComponent* _owner, std::wstring _name)
		:PlayerState(_owner, _name)
	{}
	virtual ~PlayerIdle() {};

	// PlayerState을(를) 통해 상속됨
	void Enter() override;
	void Update(float _dt) override;
	void Exit() override;

};

