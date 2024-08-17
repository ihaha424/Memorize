#pragma once
#include "BossSkillActor.h"
#include "D2DGameEngine/DamageEvent.h"
#include "CoolTime.h"


class BossThroughProjectileMagicCircle : public BossSkillActor
{
public:
	BossThroughProjectileMagicCircle(class World* _world);
	virtual ~BossThroughProjectileMagicCircle();

	virtual void BeginPlay();

	virtual void Update(float _dt) override;

	virtual void DisfellAction();
	virtual void DisfellFailAction();
	virtual void OnClicked();

	// BossSkillActor을(를) 통해 상속됨
	void ReflectionIn() override;
	void ReflectionOut() override;

public:
	class AnimationBitmapComponent* abm;
	CoolTime<void>* coolTime;

private:
	RadialDamageEvent BossThroughProjectileMagicCircleDamageEvent;
	class Player* player{ nullptr };

	Math::Vector2 _startPos;
	Math::Vector2 _direction;

};

