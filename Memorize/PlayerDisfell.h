#pragma once

#include "PlayerState.h"

class PlayerDisfell
	:public PlayerState
{
public:
	PlayerDisfell(FSMComponent* _owner, std::wstring _name)
		:PlayerState(_owner, _name)
	{}
	virtual ~PlayerDisfell() {};

	// PlayerState을(를) 통해 상속됨
	void Enter() override;
	void Update(float _dt) override;
	void Exit() override;

	virtual void Fire();
	virtual void Water();
	virtual void Light();
	virtual void Dark();
	virtual void Attack() {};
	//virtual void Move() {};
	virtual void Memorize() {};
	virtual void Teleport();
	virtual void Cancellation();


	bool	saveDirtyFlag = false;
};

