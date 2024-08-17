#pragma once

#include "BossSkillActor.h"

class ChasingProjectile : public BossSkillActor
{
public:
	ChasingProjectile(class World* _world);

	virtual void BeginPlay();

	virtual void FixedUpdate(float _fixedRate) override;

	virtual void Update(float _dt) override;

	virtual void DisfellAction();

	virtual void OnBeginOverlap(Actor* other, const OverlapInfo& overlap) override;

	virtual void OnEndOverlap(Actor* other, const OverlapInfo& overlap) override;

	// BossSkillActor을(를) 통해 상속됨
	void ReflectionIn() override;
	void ReflectionOut() override;
};
