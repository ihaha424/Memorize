#pragma once
#include "../D2DGameEngine/Character.h"
class Boss : public Character
{
	int hp;
	int maxHp = 100;

public:
	Boss(class World* _world);
	virtual ~Boss();
};

