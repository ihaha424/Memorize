#pragma once
#include "BossSkillActor.h"
#include "D2DGameEngine/DamageEvent.h"
#include "D2DGameEngine/DotTween.h"

#include "CoolTime.h"

class BossGrowCircle : public BossSkillActor
{
public:
	BossGrowCircle(class World* _world);
	virtual ~BossGrowCircle() {};

	virtual void BeginPlay() override;

	virtual void FixedUpdate(float _fixedRate) override;
	virtual void Update(float _dt) override;

	virtual void OnBeginOverlap(Actor* other) override;
	virtual void OnEndOverlap(Actor* other) override;
	// virtual void OnBeginOverlap(Actor* other, const OverlapInfo& overlap) override;

	// BossSkillActor을(를) 통해 상속됨
	void ReflectionIn() override;
	void ReflectionOut() override;

public:
	class CircleComponent* circleComponent;
	class AnimationBitmapComponent* abm;

private:
	float				startSkill = 1.f;
	float				tickInterval = 0.4f;

	RadialDamageEvent BossGrowCircleDamageEvent;
	class Player* player{ nullptr };

	float			 scaleVarias = 0.f;
	DotTween<float>* scaleTween;

	using TakeDamageTimer = CoolTime<void>;
	using FunctionTimerMap = std::unordered_map<Actor*, TakeDamageTimer>;
	FunctionTimerMap tickDamageTimerMap;
};
