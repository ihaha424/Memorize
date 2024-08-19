#pragma once

#include "BossSkillActor.h"
#include "DisfellEvent.h"
#include "D2DGameEngine/DamageEvent.h"

#include "CoolTime.h"

class RazerCircle : public BossSkillActor
{
	using Super = BossSkillActor;

	float castingTime{ 1.f };
	
	float angularVelocity{ 72.f };

	float tickInterval{ 0.4f };

	class CircleComponent* circle;
	class BossRazer* razer1;
	class BossRazer* razer2;
	class BossRazer* razer3;

	class SceneComponent* attach1;
	class SceneComponent* attach2;
	class SceneComponent* attach3;

	using TakeDamageTimer = CoolTime<void>;
	using FunctionTimerMap = std::unordered_map<Actor*, TakeDamageTimer>;
	FunctionTimerMap tickDamageTimerMap;
	RadialDamageEvent circleDamage;
public:
	bool destroyThis{ false };

	RazerCircle(class World* _world);
	virtual ~RazerCircle() {};

	virtual void BeginPlay();

	virtual void FixedUpdate(float _fixedRate) override;

	virtual void Update(float _dt) override;

	virtual void DisfellAction();

	virtual void OnBeginOverlap(Actor* other, const OverlapInfo& overlap) override;

	virtual void OnEndOverlap(Actor* other, const OverlapInfo& overlap) override;

	// BossSkillActor을(를) 통해 상속됨
	void ReflectionIn() override;
	void ReflectionOut() override;

	void DestroyThis();
};