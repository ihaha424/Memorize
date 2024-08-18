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

public:
	MagicBinding(class World* _world);
	virtual ~MagicBinding() {};

	virtual void BeginPlay();

	virtual void FixedUpdate(float _fixedRate) override;

	virtual void Update(float _dt) override;

	virtual void DisfellAction();

	void ReflectionIn() override;
	void ReflectionOut() override;
};

