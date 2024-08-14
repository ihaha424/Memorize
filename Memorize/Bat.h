#pragma once

#include "common.h"

#include "BossSkillActor.h"
#include "D2DGameEngine/Reflection.h"
#include "D2DGameEngine/DamageEvent.h"

class Bat : public BossSkillActor
{
	LOG_REGISTER_OBJ(Bat)
	using Super = BossSkillActor;
	class AnimationBitmapComponent* moveAnimation{ nullptr };
	class AnimationBitmapComponent* attackAnimation{ nullptr };
	class Player* player{ nullptr };
	float attackRange{ 100.f };
	float speed{ 250.f };

	PointDamageEvent batDamageEvent;
public:
	Bat(World* world);

	void BeginPlay() override;

	void OnBeginOverlap(Actor* other) override;

	void Update(float _dt) override;

	void ReflectionIn() override;
	void ReflectionOut() override {};
};
