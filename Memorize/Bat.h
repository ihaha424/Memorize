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

	float 추적_타이머{ 3.f };
public:

	float hp = 40.f;
	float	damage = 5.f;
	float attackRange{ 100.f };
	float attackSpeed{ 1.f };
	float speed{ 250.f };

	Bat(World* world);

	void BeginPlay() override;

	void OnTakeDamage(float damageAmount, struct DamageEvent const& damageEvent, class Controller* eventInstigator, Actor* damageCauser);

	void Update(float _dt) override;

	void ReflectionIn() override;
	void ReflectionOut() override {};
};
