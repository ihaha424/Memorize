#pragma once
#include "D2DGameEngine/Actor.h"

class LightStreamEffect : public Actor
{
	class BitmapComponent* bm;
	float duration = 1.f;
	float elapsedTime = 0.f;
	float damage = 0.f;
	class BoxComponent* box;
public:
	LightStreamEffect(World* _world);
	virtual ~LightStreamEffect();

	void SetDamage(float _damage) { damage = _damage; }
	void OnBeginOverlap(Actor* other, const OverlapInfo& overlap);

	void SetDuration(float _duration) { duration = _duration; };
	virtual void BeginPlay() override;
	virtual void Update(float _dt) override;
};

