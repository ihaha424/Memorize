#pragma once

#include "BossSkillActor.h"
#include "DisfellEvent.h"
#include "D2DGameEngine/DamageEvent.h"

#include "CoolTime.h"

class BossRazer : public BossSkillActor
{
	LOG_REGISTER_OBJ(BossRazer)
	using Super = BossSkillActor;

	class Player* player{ nullptr };

	class BitmapComponent* magicCircleHub;

	bool startCharging{ false };
	class AnimationBitmapComponent* lazerCharging;
	class AnimationBitmapComponent* lazer;
	bool startShuttingDown{ false };
	float shuttingDownTimer{ 1.f };
	class AnimationBitmapComponent* lazerShuttingDown;

	class PolygonComponent* obb;
	
	using TakeDamageTimer = CoolTime<void>;

	using FunctionTimerMap = std::unordered_map<Actor*, TakeDamageTimer>;
	FunctionTimerMap tickDamageTimerMap;
	RadialDamageEvent razerDamageEvent;

	bool destroyThis{ false };
public:
	float castTime{ 0.f };
	float tickInterval{ 0.4 };
	class AnimationBitmapComponent* magicCircle;
	class ClickComponent* clickComp;
	BossRazer(class World* _world);
	virtual ~BossRazer();

	virtual void BeginPlay();

	virtual void FixedUpdate(float _fixedRate) override;

	virtual void Update(float _dt) override;
	virtual bool Destroy() override;

	virtual void DisfellAction() override;
	virtual void DisfellFailAction() override;

	void OnClicked() override;

	virtual void OnBeginOverlap(Actor* other, const OverlapInfo& overlap) override;

	virtual void OnEndOverlap(Actor* other, const OverlapInfo& overlap) override;

	// BossSkillActor을(를) 통해 상속됨
	void ReflectionIn() override;
	void ReflectionOut() override;

	void DestroyThis();
};