#pragma once
#include "../D2DGameEngine/Actor.h"

class Projectile : public Actor
{
protected:
	class Character* owner = nullptr;
	int damage = 0;
	float speed;
	float duration = 2.f;

	float delay = 0.f;

	float elapsedTime = 0.f;
	bool bLaunched = false;

	class MovementComponent* mv;
	class BitmapComponent* bm;
	class BoxComponent* box;
public:
	Projectile(class World* _world);
	virtual ~Projectile();

	virtual void OnBeginOverlap(Actor* other) override;

	void SetDamage(float _damage) { damage = _damage; }
	void SetDelay(float _delay) { delay = _delay; }
	void SetOwner(class Character* _owner) { owner = _owner; }
	void SetVelocity(Math::Vector2 _direction, float _speed);

	virtual void BeginPlay() override;
	virtual void Initialize() {}
	void Update(float _dt) override;
};

