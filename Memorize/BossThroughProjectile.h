#pragma once

#include "BossSkillActor.h"
#include "D2DGameEngine/DamageEvent.h"
#include "D2DGameEngine/DotTween.h"


class BossThroughProjectile : public BossSkillActor
{
public:
	BossThroughProjectile(class World* _world);
	virtual ~BossThroughProjectile() {/*∆¯∆»¿Ã∆Â∆Æ ? */ };

	virtual void BeginPlay();
	virtual void FixedUpdate(float _fixedRate) override;
	virtual void Update(float _dt) override;

	virtual void OnBeginOverlap(Actor* other, const OverlapInfo& overlap) override;


	// BossSkillActor¿ª(∏¶) ≈Î«ÿ ªÛº”µ 
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
	bool destroyThis{ false };
};

