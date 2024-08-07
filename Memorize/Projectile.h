#pragma once
#include "../D2DGameEngine/Actor.h"

class Projectile : public Actor
{
	float speed = 10.f;
public:
	Projectile(class World* _world);
	virtual ~Projectile();
};

