#pragma once
#include "Projectile.h"
#include "../D2DGameEngine/DotTween.h"

class ChasingWaterBallProjectile : public Projectile
{
	enum State{Idle, Chase, Boom};
	State state = State::Idle;

	float			x = 0.f;
	float			y = 0.f;
	//DotTween<float> xValue;
	//DotTween<float> yValue;
	class Actor* target;
	class CircleComponent* rangeCircle;
	float range = 500.f;
	static std::vector<class Actor*> chasingEnemies;
public:
	ChasingWaterBallProjectile(class World* _world);
	virtual ~ChasingWaterBallProjectile();

	virtual void Update(float _dt) override;

	void SetInitialLocation(float _x, float _y) { x = _x, y = _y; }
	// Projectile을(를) 통해 상속됨
	void Initialize() override;
};

