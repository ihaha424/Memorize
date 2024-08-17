#pragma once
#include "BossSkillActor.h"
#include "D2DGameEngine/DamageEvent.h"
#include "D2DGameEngine/DotTween.h"

class BossGrowCircle : public BossSkillActor
{
public:
	BossGrowCircle(class World* _world);
	virtual ~BossGrowCircle() {};

	virtual void BeginPlay() override;

	virtual void FixedUpdate(float _fixedRate) override;
	virtual void Update(float _dt) override;

	virtual void OnBeginOverlap(Actor* other) override;

	// BossSkillActor을(를) 통해 상속됨
	void ReflectionIn() override;
	void ReflectionOut() override;

public:
	class CircleComponent* circleComponent;

private:
	float				startSkill = 1.f;


	RadialDamageEvent BossGrowCircleDamageEvent;
	class Player* player{ nullptr };

	float			 scaleVarias = 0.f;
	DotTween<float>* scaleTween;
};
