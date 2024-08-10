#pragma once
#include "D2DGameEngine/Actor.h"

class MeteorEffect : public Actor
{
	class BitmapComponent* bitmap;
	class MovementComponent* movement;
	float tighteningTime = 2.f;
	float elapsedTime = 0.f;
	Math::Vector2 attackPos;
public:
	MeteorEffect(World* _world);
	virtual ~MeteorEffect();

	virtual void BeginPlay() override;
	virtual void Update(float _dt) override;

	void Initialize();
	void SetAttackPos(Math::Vector2 _attackPos) { attackPos = _attackPos; }
};

