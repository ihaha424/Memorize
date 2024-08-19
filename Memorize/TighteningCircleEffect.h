#pragma once
#include "D2DGameEngine/Actor.h"
#include "D2DGameEngine/DamageEvent.h"
#include "D2DGameEngine/DotTween.h"
#include "CoolTime.h"

class TighteningCircleEffect : public Actor
{
public:
	TighteningCircleEffect(class World* _world);
	virtual ~TighteningCircleEffect() {};
	virtual void BeginPlay() override;

	void Initialize();
	virtual void Update(float _dt) override;

	virtual void OnOverlap(Actor* other, const OverlapInfo& overlap) override;
	
	void SetDuration(float _duration) { skillDuration = _duration; }
	void SetDamage(float _damage) { damage = _damage; }

public:
	class CircleComponent* circleComponent;
	class AnimationBitmapComponent* abm;

private:
	float	tickInterval = 0.2f;
	float	radius = 250.f;
	float	damage = 0.f;
	float	skillDuration = 2.f;
	float elapsedTime = 0.f;
	float damageTimer = 0.f;

	RadialDamageEvent tighteningDamageEvent;

	float			 scaleVarias = 0.f;
	DotTween<float>* scaleTween;

};

