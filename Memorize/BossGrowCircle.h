#pragma once
#include "BossSkillActor.h"
#include "D2DGameEngine/DamageEvent.h"
#include "D2DGameEngine/DotTween.h"
#include "D2DGameEngine/Ellipse.h"

#include "CoolTime.h"

class BossGrowCircle : public BossSkillActor
{
	using Super = BossSkillActor;
public:
	BossGrowCircle(class World* _world);
	virtual ~BossGrowCircle() {};

	virtual void BeginPlay() override;

	virtual void FixedUpdate(float _fixedRate) override;
	virtual void Update(float _dt) override;
	virtual void Render(class D2DRenderer* _renderer) override;
	virtual bool Destroy() override;

	virtual void OnBeginOverlap(Actor* other, const OverlapInfo& overlap) override;
	virtual void OnEndOverlap(Actor* other, const OverlapInfo& overlap) override;

	void SetGrowLess(bool _Grow);

	// BossSkillActor을(를) 통해 상속됨
	void ReflectionIn() override;
	void ReflectionOut() override;

public:
	//class CircleComponent* circleComponent;
	TEllipse ellipse;
	float minor{ 800.f };
	float major{ 1200.f };
	class AnimationBitmapComponent* abm;

private:
	float				startSkill = 1.f;
	float				tickInterval = 0.4f;
	float				radius = 500.f;
	bool				isReverse{ false };

	RadialDamageEvent BossGrowCircleDamageEvent;
	class Player* player{ nullptr };

	float			 scaleVarias = 0.f;
	DotTween<float>* scaleTween;

	using TakeDamageTimer = CoolTime<void>;
	using FunctionTimerMap = std::unordered_map<Actor*, TakeDamageTimer>;
	FunctionTimerMap tickDamageTimerMap;
};
