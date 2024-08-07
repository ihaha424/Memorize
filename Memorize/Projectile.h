#pragma once
#include "../D2DGameEngine/Actor.h"

class Projectile : public Actor
{
	class Character* owner = nullptr;
	int damage = 0;

public:
	Projectile(class World* _world, class Character* _owner);
	virtual ~Projectile();

	void SetVelocity(Math::Vector2 _direction, float _speed);
};

