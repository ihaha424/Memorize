#pragma once

#include "PlayerState.h"

class PlayerBlinking
	:public PlayerState
{
public:
	PlayerBlinking(FSMComponent* _owner, std::wstring _name)
		:PlayerState(_owner, _name)
	{}
	virtual ~PlayerBlinking() {};

	static constexpr float blinkTime = 1.f;
	static constexpr float blinkDistance = 550.f;

	float curtime = 0.f;
	Math::Vector2 worldMousePos;

	// PlayerState을(를) 통해 상속됨
	void Enter() override;
	void Update(float _dt) override;
	void Exit() override;


	virtual void Fire() {};
	virtual void Water() {};
	virtual void Light() {};
	virtual void Dark() {};
	virtual void Attack() {};
	virtual void Move() {};
	virtual void Memorize() {};
	virtual void Teleport() {};
	virtual void Cancellation() {};
};

