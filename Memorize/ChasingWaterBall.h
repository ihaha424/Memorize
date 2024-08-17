#pragma once
#include "ProjectileSkill.h"

class ChasingWaterBall : public ProjectileSkill
{
	float speed = 10.f;

	std::vector<Math::Vector2> locations;
	
public:
	ChasingWaterBall(Actor* _owner);
	virtual ~ChasingWaterBall();


	virtual void UseSkill() override;
	virtual void ReflectionIn() override;

};

