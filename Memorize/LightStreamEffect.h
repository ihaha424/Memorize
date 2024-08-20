#pragma once
#include "SkillActor.h"

class LightStreamEffect : public SkillActor
{
	enum class State
	{
		Initial, Normal, Ending
	};
	class Animator* anim;
	float duration = 1.f;
	float initialTime = 2.f;
	float endingTime = 1.f; 
	float elapsedTime = 0.f;
	float damage = 0.f;

	class PolygonComponent* obb;
	class AnimationState* initialState;
	class AnimationState* normalState;
	class AnimationState* endingState;
	State state = State::Initial;

	float damageTimer = 0.f;
	float damageInterval = 0.2f;

public:
	LightStreamEffect(World* _world);
	virtual ~LightStreamEffect();

	void SetDamage(float _damage) { damage = _damage; }
	void OnOverlap(Actor* other, const OverlapInfo& overlap);

	void SetDuration(float _duration) { duration = _duration; };
	void Initialize();
	virtual void BeginPlay() override;
	virtual void Update(float _dt) override;
};

