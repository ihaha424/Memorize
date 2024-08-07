#pragma once
#include "Projectile.h"
#include "../D2DGameEngine/DotTween.h"

class ChasingWaterBallProjectile : public Projectile
{
	enum State{Idle, Chase, Boom};
	State state = State::Idle;

	float			x = 0.f;
	float			y = 0.f;
	DotTween<float> xValue;
	DotTween<float> yValue;
public:
	ChasingWaterBallProjectile(class World* _world);
	virtual ~ChasingWaterBallProjectile();

	virtual void Update(float _dt) override;

	// Projectile을(를) 통해 상속됨
	void Initialize() override;
};

