#pragma once
#include "SkillActor.h"

class Projectile : public SkillActor
{
protected:
	class Player* player = nullptr;
	int damage = 0;
	float speed;
	float duration = 2.f;
	float endingTime = 0.0f;
	float delay = 0.f;
	float elapsedTime = 0.f;

	bool bLaunched = false;
	bool bEnding = false;
	bool bHasEnding = false;
	bool bIsPassable = false;
	bool bCollideWithOtherAttack = false;

	class MovementComponent* mv;
	class Animator* anim;
	class PolygonComponent* box;

	class AnimationState* normalState;
	class AnimationState* endingState;
public:
	Projectile(class World* _world);
	virtual ~Projectile();

	virtual void OnBeginOverlap(Actor* other, const OverlapInfo& overlap) override;

	void SetDamage(float _damage) { damage = _damage; }
	void SetDelay(float _delay) { delay = _delay; }
	void SetDuration(float _duration) { duration = _duration; }
	void SetPlayer(class Player* _player) { player = _player; }
	void SetSpeed(float _speed) { speed = _speed; }
	void SetVelocity(Math::Vector2 _direction, float _speed);

	virtual void BeginPlay() override;
	virtual void Initialize();
	virtual void FixedUpdate(float _fixedRate) override;
	void Update(float _dt) override;
};

