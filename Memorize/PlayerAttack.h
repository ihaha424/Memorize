#pragma once

#include "PlayerState.h"

class PlayerAttack
	:public PlayerState
{
public:
	PlayerAttack(FSMComponent* _owner, std::wstring _name)
		:PlayerState(_owner, _name)
	{}
	virtual ~PlayerAttack() {};

	// PlayerState을(를) 통해 상속됨
	void Enter() override;
	void Update(float _dt) override;
	void Exit() override;

	virtual void Attack();
	virtual void Memorize();

};

