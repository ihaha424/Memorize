#pragma once

#include "BossSkillActor.h"
#include "D2DGameEngine/DamageEvent.h"
#include "D2DGameEngine/DotTween.h"


class BossThroughProjectile : public BossSkillActor
{
public:
	BossThroughProjectile(class World* _world);
	virtual ~BossThroughProjectile() {};

	virtual void BeginPlay();

	virtual void Update(float _dt) override;


	// BossSkillActor을(를) 통해 상속됨
	void ReflectionIn() override;
	void ReflectionOut() override;

	void SetPosAndDerection(Math::Vector2 _startPos, Math::Vector2 _direction);

public:
	class CircleComponent* circleComponent;
	class AnimationBitmapComponent* abm;

private:
	RadialDamageEvent BossThroughProjectileDamageEvent;
	class Player* player{ nullptr };

	float			 speedVarias = 0.f;
	DotTween<float>* speedTween;
};

