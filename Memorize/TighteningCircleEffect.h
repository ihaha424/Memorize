#pragma once
#include "D2DGameEngine/Actor.h"

class TighteningCircleEffect : public Actor
{
	class BitmapComponent* bitmap;
	class CapsuleComponent* capsule;
	float tighteningTime = 2.f;
	float elapsedTime = 0.f;
public:
	TighteningCircleEffect(World* _world);
	virtual ~TighteningCircleEffect();

	virtual void BeginPlay() override;
	virtual void Update(float _dt) override;

	void Initialize();
};

