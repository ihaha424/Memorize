#pragma once
#include "D2DGameEngine/Actor.h"

class LightStreamEffect : public Actor
{
	class BitmapComponent* bm;
	float duration = 1.f;
	float elapsedTime = 0.f;
public:
	LightStreamEffect(World* _world);
	virtual ~LightStreamEffect();

	virtual void BeginPlay() override;
	virtual void Update(float _dt) override;
};

