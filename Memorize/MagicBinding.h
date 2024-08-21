#pragma once

#include "BossSkillActor.h"
#include "DisfellEvent.h"
#include "D2DGameEngine/DamageEvent.h"

#include "CoolTime.h"

class MagicBinding : public BossSkillActor
{
	LOG_REGISTER_OBJ(MagicBinding)
	using Super = BossSkillActor;

	class Player* player{ nullptr };

	class AnimationBitmapComponent* floorChaining;
	class AnimationBitmapComponent* floorChained;
	class AnimationBitmapComponent* bodyChaining;
	class AnimationBitmapComponent* bodyChained;


	float chainingTime;
	bool chained{ false };

	float releasingTime{ 1.f };
	bool release{ false };

	float retryCooldown{ 0.f };
	bool failed{ false };

public:
	MagicBinding(class World* _world);
	virtual ~MagicBinding();

	virtual void BeginPlay();

	virtual void FixedUpdate(float _fixedRate) override;

	virtual void Update(float _dt) override;

	virtual bool Destroy() override;

	virtual void DisfellAction() override;
	virtual void DisfellFailAction() override;

	void ReflectionIn() override;
	void ReflectionOut() override;
};

