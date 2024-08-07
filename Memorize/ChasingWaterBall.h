#pragma once
#include "ProjectileSkill.h"

class ChasingWaterBall : public ProjectileSkill
{
	int maxCount = 6;
	int nowCount = 0;
	int onceClickCount = 3;
	float speed = 10.f;
public:
	ChasingWaterBall(class World* _world);
	virtual ~ChasingWaterBall();

	virtual void UseSkill() override;

};

