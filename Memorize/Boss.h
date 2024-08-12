#pragma once
#include "../D2DGameEngine/Character.h"
class Boss : public Character
{
public:
	int hp;
	int maxHp = 100;
	int DissfellCount = 0;
	float Periodic_Pattern_Cool_Time = 0.f;

public:
	Boss(class World* _world);
	virtual ~Boss();

	virtual void Update(float _dt) override;
};

