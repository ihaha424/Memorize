#pragma once
#include "../D2DGameEngine/Actor.h"

class Projectile : public Actor
{
protected:
	class Character* owner = nullptr;
	int damage = 0;

public:
	Projectile(class World* _world);
	virtual ~Projectile();

	void SetOwner(class Character* _owner) { owner = _owner; }
	void SetVelocity(Math::Vector2 _direction, float _speed);
	virtual void Initialize() {}
};

