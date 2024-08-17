#pragma once

#include "common.h"

#include "D2DGameEngine/Actor.h"
#include "D2DGameEngine/Reflection.h"
#include "D2DGameEngine/DamageEvent.h"

class Bat : public Actor, public IReflection
{
	LOG_REGISTER_OBJ(Bat)
	using Super = Actor;
	class AnimationBitmapComponent* moveAnimation{ nullptr };
	class AnimationBitmapComponent* attackAnimation{ nullptr };
	class Player* player{ nullptr };
	
	PointDamageEvent batDamageEvent;
public:

	float	damage = 0.f;
	float attackRange{ 100.f };
	float attackSpeed{ 1.f };
	float speed{ 250.f };

	Bat(World* world);

	void BeginPlay() override;

	void OnBeginOverlap(Actor* other, const OverlapInfo& overlap) override;

	void Update(float _dt) override;

	void ReflectionIn() override;
	void ReflectionOut() override {};
};
