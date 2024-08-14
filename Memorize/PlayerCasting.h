#pragma once

#include "PlayerState.h"

class PlayerCasting
	:public PlayerState
{
public:
	PlayerCasting(FSMComponent* _owner, std::wstring _name)
		:PlayerState(_owner, _name)
	{}
	virtual ~PlayerCasting() {};

	// PlayerState을(를) 통해 상속됨
	void Enter() override;
	void Update(float _dt) override;
	void Exit() override;
	
	virtual void Fire();
	virtual void Water();
	virtual void Light();
	virtual void Dark();
	virtual void Attack() {};

	std::vector<BYTE>	commandList{};
	size_t				index = 1;
	//or queue

};

