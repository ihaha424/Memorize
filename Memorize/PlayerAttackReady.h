#pragma once

#include "PlayerState.h"

class PlayerAttackReady
	:public PlayerState
{
public:
	PlayerAttackReady(FSMComponent* _owner, std::wstring _name)
		:PlayerState(_owner, _name)
	{}
	virtual ~PlayerAttackReady() {};

	// PlayerState을(를) 통해 상속됨
	void Enter() override;
	void Update(float _dt) override;
	void Exit() override;

	virtual void Fire() {};
	virtual void Water() {};
	virtual void Light() {};
	virtual void Dark() {};
	virtual void Attack();
	virtual void Memorize();
};

