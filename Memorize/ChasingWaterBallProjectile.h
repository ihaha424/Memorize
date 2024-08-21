#pragma once
#include "Projectile.h"
#include "../D2DGameEngine/DotTween.h"

class ChasingWaterBallProjectile : public Projectile
{
	enum State { Idle, Chase, Boom };
	State state = State::Idle;

	float			x = 0.f;
	float			y = 0.f;

	Math::Vector2 startPos;
	Math::Vector2 targetPos;
	Math::Vector2 controlPoint;
	Math::Vector2 prevPos;
	float chaseDelay = 0.f;
	float t = 0.f;
	float range = 500.f;
	std::vector<class Actor*> chasingEnemies;

	class AnimationState* chaseState;
public:
	ChasingWaterBallProjectile(class World* _world);
	virtual ~ChasingWaterBallProjectile();

	virtual void OnBeginOverlap(Actor* other, const OverlapInfo& overlap) override;
	virtual void Update(float _dt) override;

	void SetInitialLocation(float _x, float _y) { x = _x, y = _y; }
	Math::Vector2 CalculateBezierPoint(float t, const Math::Vector2& p0, const Math::Vector2& p1, const Math::Vector2& p2);
	void SetChaseDelay(float _chaseDelay) { chaseDelay = _chaseDelay; }
};