#pragma once

#include "../D2DGameEngine/FSMState.h"

class PlayerState
	:public FSMState
{
public:
	PlayerState(FSMComponent* _owner, std::wstring _name)
	:FSMState(_owner, _name)
	{}
	~PlayerState() {}


	virtual void Q();
	virtual void W();
	virtual void E();
	virtual void R();
	virtual void Left();
	virtual void Right();

	// FSMState을(를) 통해 상속됨
	virtual void Enter() = 0;
	virtual void Update(float _dt) = 0;
	virtual void Exit() = 0;
};

