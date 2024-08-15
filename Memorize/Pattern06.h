#pragma once
#include "BossSkillActor.h"
#include "DisfellEvent.h"
#include "D2DGameEngine/DamageEvent.h"

class Pattern06 : public BossSkillActor
{
public:
	Pattern06(class World* _world);
	virtual ~Pattern06() {};

	virtual void BeginPlay();

	virtual void Update(float _dt) override;

	virtual void DisfellAction();


	// BossSkillActor을(를) 통해 상속됨
	void ReflectionIn() override;
	void ReflectionOut() override;

public:
	class CircleComponent* circleComponent;

private:
	DisFellEvent Pattern06DissfellEvent;
	RadialDamageEvent Pattern06DamageEvent;
	class Player* player{ nullptr };
};

