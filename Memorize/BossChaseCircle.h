#pragma once
#include "BossSkillActor.h"
#include "DisfellEvent.h"
#include "D2DGameEngine/DamageEvent.h"
#include "D2DGameEngine/DotTween.h"

class BossChaseCircle : public BossSkillActor
{
public:
	BossChaseCircle(class World* _world);
	virtual ~BossChaseCircle();

	virtual void BeginPlay();

	virtual void Update(float _dt) override;
	virtual bool Destroy() override;


	virtual void DisfellAction() override;
	virtual void DisfellFailAction() override;
	virtual void OnClicked() override;


	// BossSkillActor을(를) 통해 상속됨
	void ReflectionIn() override;
	void ReflectionOut() override;

public:
	class CircleComponent* circleComponent;
	class AnimationBitmapComponent* magicCircle;

	bool ignited{ false };
	float explosionTimer{ 1.f };

private:
	RadialDamageEvent BossChaseCircleDamageEvent;
	class Player* player{ nullptr };

	float			 speedVarias = 0.f;
	DotTween<float>* speedTween;

	float			 scaleVarias = 0.f;
	DotTween<float>* scaleTween;


	bool bShutdownDispelChannel{ false };
	void ShutdownDispelChannel();
};

