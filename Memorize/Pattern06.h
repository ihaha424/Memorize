#pragma once
#include "BossSkillActor.h"

class Pattern06 : public BossSkillActor
{
public:
	Pattern06(class World* _world);
	virtual ~Pattern06() {};

	virtual void BeginPlay();

	virtual void Update(float _dt) override;


	// BossSkillActor을(를) 통해 상속됨
	void ReflectionIn() override;
	void ReflectionOut() override;

public:
	class CircleComponent* circleComponent;
};

