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

	float skillDelay = 0.f;

	virtual void Fire(){};
	virtual void Water(){};
	virtual void Light(){};
	virtual void Dark(){};
	virtual void Attack(){};
	virtual void Move(){};
	virtual void Memorize(){};
	virtual void Teleport(){};
	virtual void Cancellation(){};

	int LvelUpEffect[12]{0,2,2,3,0,2,0,3,1,1,1,3};
	int LvelUp[12]{ 5,10,20,20,25,25,30,30,40,50,50,50 };
	int index = 0;

};

