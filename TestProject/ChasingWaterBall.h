#pragma once
#include "../D2DGameEngine/Actor.h"
#include "../D2DGameEngine/DotTween.h"

#include "../D2DGameEngine/BitmapComponent.h"
#include "../D2DGameEngine/Mouse.h"
#include "../D2DGameEngine/Character.h"

#include "../D2DGameEngine/SceneComponent.h"

class MovementComponent : public SceneComponent
{
public:
	MovementComponent() { SetTickProperties(TICK_UPDATE); }
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


class Projectile : public Actor
{
protected:
	class Character* owner = nullptr;
	int damage = 0;

public:
	Projectile(class World* _world, class Character* _owner);
	virtual ~Projectile();

	void SetVelocity(Math::Vector2 _direction, float _speed);
	virtual void Initialize() = 0;
};



Projectile::Projectile(World* _world, class Character* _owner) : Actor(_world)
{
	owner = _owner;
	CreateComponent<BitmapComponent>();
	CreateComponent<MovementComponent>();
}

Projectile::~Projectile()
{
}

void Projectile::SetVelocity(Math::Vector2 _direction, float _speed)
{
	MovementComponent* mv = GetComponent<MovementComponent>();
	mv->SetDirection(_direction);
	mv->SetSpeed(_speed);
}


class ChasingWaterBallProjectile : public Projectile
{
	enum State { Idle, Chase, Boom };
	State state = State::Idle;

	float			x = 0.f;
	float			y = 0.f;
	DotTween<float> xValue;
	DotTween<float> yValue;
public:
	ChasingWaterBallProjectile(class World* _world, class Character* _owner);
	virtual ~ChasingWaterBallProjectile();

	virtual void Update(float _dt) override;

	// Projectile을(를) 통해 상속됨
	void Initialize() override;
};

