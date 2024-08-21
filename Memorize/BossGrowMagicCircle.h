#pragma once
#include "BossSkillActor.h"
#include "DisfellEvent.h"
#include "D2DGameEngine/DamageEvent.h"

class BossGrowMagicCircle : public BossSkillActor
{
public:
	BossGrowMagicCircle(class World* _world);
	virtual ~BossGrowMagicCircle();

	virtual void BeginPlay();

	virtual void Update(float _dt) override;
	virtual bool Destroy() override;

	virtual void DisfellOneCountAction();
	virtual void DisfellAction();
	virtual void DisfellFailAction();
	virtual void OnClicked();


	// BossSkillActor��(��) ���� ��ӵ�
	void ReflectionIn() override;
	void ReflectionOut() override;

public:
	class CircleComponent* circleComponent;
	class AnimationBitmapComponent* abm;

private:
	RadialDamageEvent BossGrowMagicCircleDamageEvent;
	class Player* player{ nullptr };
};

