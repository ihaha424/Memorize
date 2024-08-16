#pragma once
#include "BossSkillActor.h"
#include "DisfellEvent.h"
#include "D2DGameEngine/DamageEvent.h"
#include "D2DGameEngine/DotTween.h"

class BossChaseCircle : public BossSkillActor
{
public:
	BossChaseCircle(class World* _world);
	virtual ~BossChaseCircle() {};

	virtual void BeginPlay();

	virtual void Update(float _dt) override;


	virtual void DisfellAction();
	virtual void OnClicked();


	// BossSkillActor��(��) ���� ��ӵ�
	void ReflectionIn() override;
	void ReflectionOut() override;

public:
	class CircleComponent* circleComponent;

private:
	RadialDamageEvent BossChaseCircleDamageEvent;
	class Player* player{ nullptr };

	float			 speedVarias = 0.f;
	DotTween<float>* speedTween;


	float			 scaleVarias = 0.f;
	DotTween<float>* scaleTween;
};

