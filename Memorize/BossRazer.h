#pragma once

#include "BossSkillActor.h"
#include "DisfellEvent.h"
#include "D2DGameEngine/DamageEvent.h"

#include "CoolTime.h"

class BossRazer : public BossSkillActor
{
	LOG_REGISTER_OBJ(BossRazer)
	using Super = BossSkillActor;

	RadialDamageEvent BossGrowMagicCircleDamageEvent;
	class Player* player{ nullptr };

	class AnimationBitmapComponent* razer;
	class SceneComponent* magicCircleHub;
	class BitmapComponent* magicCircle1;
	class BitmapComponent* magicCircle2;
	class BitmapComponent* magicCircle3;
	class BitmapComponent* magicCircle4;

	class PolygonComponent* obb;

	float castTime{ 0.f };
	float tickInterval{ 0.4 };
	
	using TakeDamageTimer = CoolTime<void>;

	using FunctionTimerMap = std::unordered_map<Actor*, TakeDamageTimer>;
	FunctionTimerMap tickDamageTimerMap;
	RadialDamageEvent razerDamageEvent;

public:
	BossRazer(class World* _world);
	virtual ~BossRazer() {};

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