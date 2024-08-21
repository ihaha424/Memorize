#pragma once

#include "BossSkillActor.h"
#include "D2DGameEngine/DamageEvent.h"

#include "BossSkillActor.h"

class ChasingBomb : public BossSkillActor
{
	LOG_REGISTER_OBJ(ChasingBomb)
	using Super = BossSkillActor;

	class Player* player{ nullptr };

	class AnimationBitmapComponent* chasingBomb;
	class CircleComponent* bombTrigger;
	class AnimationBitmapComponent* explosionEffect;
	class CircleComponent* bombExplosionRadius;

	float explodingTime{ 0.1f };
	bool exploding{ false };
	
	PointDamageEvent bombDamageEvent;

public:
	bool destroyThis{ false };

	ChasingBomb(class World* _world);

	virtual void BeginPlay();

	virtual void FixedUpdate(float _fixedRate) override;

	virtual void Update(float _dt) override;

	virtual bool Destroy() override;

	virtual void DisfellAction();

	virtual void OnBeginOverlap(Actor* other, const OverlapInfo& overlap) override;

	virtual void OnEndOverlap(Actor* other, const OverlapInfo& overlap) override;

	// BossSkillActor을(를) 통해 상속됨
	void ReflectionIn() override;
	void ReflectionOut() override;

	void DistroyThis();
};
