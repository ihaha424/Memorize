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

	class BossSkillActor* targetSkill;

	// PlayerState��(��) ���� ��ӵ�
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
	//virtual void Teleport() {};
	virtual void Cancellation();

};

