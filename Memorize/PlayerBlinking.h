#pragma once

#include "PlayerState.h"
#include "../D2DGameEngine/Line.h"

class PlayerBlinking
	:public PlayerState
{
public:
	PlayerBlinking(FSMComponent* _owner, std::wstring _name)
		:PlayerState(_owner, _name)
	{}
	virtual ~PlayerBlinking() {};

	static constexpr float blinkTime = 0.1f;
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

private:
	Math::Vector2 ClampPointInDiamond(Math::Vector2 point);
	Line positiveSlopeUp { .start = {-3120.f,180.f}, .end = {0.f,-1780.f} };
	Line positiveSlopeDwon { .start = {0.f,2140.f},.end = {3120.f,180.f} };
	Line negativeSlopeUp { .start = {0.f,-1780.f},.end = {3120.f,180.f} };
	Line negativeSlopeDown { .start = {-3120.f,180.f},.end = {0.f,2140.f} };
};

