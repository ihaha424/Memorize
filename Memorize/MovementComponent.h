#pragma once
#include "../D2DGameEngine/SceneComponent.h"

class MovementComponent : public SceneComponent
{
public:
	MovementComponent(Actor* _owner) : SceneComponent(_owner)
	{ SetTickProperties(TICK_UPDATE); }
	virtual ~MovementComponent() {}

	void Update(float _dt);

	void SetDirection(Math::Vector2 _dir) { direction = _dir; }
	void SetSpeed(float _speed) { speed = _speed; }
	float GetSpeed() { return speed; }
	Math::Vector2 GetDirection() { return direction; }

private:
	Math::Vector2 direction = { 0,0 };
	float speed = 0.f;
};

