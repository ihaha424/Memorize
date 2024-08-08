#pragma once
#include "ProjectileSkill.h"

class ChasingWaterBall : public ProjectileSkill
{
	int maxCount = 6;
	int nowCount = 0;
	int onceClickCount = 3;
	float speed = 10.f;
public:
	ChasingWaterBall(Actor* _owner);
	virtual ~ChasingWaterBall();

	virtual void BeginPlay() override;
	virtual void UseSkill() override;

};

