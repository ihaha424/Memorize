#pragma once
#include "../D2DGameEngine/Character.h"

#include "D2DGameEngine/Debug.h"
#include "Signal.h"

class Boss : public Character
{
	LOG_REGISTER_OBJ(Boss)

public:
	int hp;
	int maxHp = 10000;
	int DissfellCount = 0;
	float Periodic_Pattern_Cool_Time = 0.f;		//메인 패턴의 딜레이
	float Phase_Pattern_Cool_Time = 0.f;		//각 페이즈의 특수 패턴 딜레이
	float Pattern_Delay = 0.f;					//각 패턴의 딜레이
	float Detection_Range = 880.f;
	float Avoidance_Range = 450.f;

	float elapsedTime{ 0.f };

	bool casting{ false };

public:
	Boss(class World* _world);
	virtual ~Boss();

	virtual void Update(float _dt) override;

	virtual void OnHit(PrimitiveComponent* myComp, PrimitiveComponent* otherComp, bool bSelfMoved, const HitResult& hitResult) override;

	virtual void OnBeginOverlap(Actor* other) {
		OBJ_MESSAGE("Overlap began!");
	}

	virtual void OnEndOverlap(Actor* other) {
		OBJ_MESSAGE("Overlap ended!");
	}

	virtual void OnTakeDamage(float damageAmount, struct DamageEvent const& damageEvent, class Controller* eventInstigator, Actor* damageCauser) override;

	//Signal for UI
	Signal<float>* OnHPChanged;

public:
	class Animator* abm;
	class AnimationState* IdleAnimationState;
	class AnimationState* MoveAnimationState;
	class AnimationState* CastingAnimationState;
	class AnimationState* DieAnimationState;
	class AnimationState* TeleportStartAnimationState;
	class AnimationState* TeleportEndAnimationState;

};

