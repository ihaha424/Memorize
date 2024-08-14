#pragma once
#include "../D2DGameEngine/Actor.h"
#include "../D2DGameEngine/Reflection.h"

class BossSkillActor : public Actor, public IReflection
{
protected:
	enum BossSkillType { Projectile, Range };

public:
	BossSkillActor(class World* _world);
	virtual ~BossSkillActor();

protected:
	bool			isDispel = false;
	bool			isFragile = false;
	BossSkillType	type = Projectile;
	float			dispelTime = 0.f;
	float			skillDuration = 0.f;
	// 스킬 시전 타이밍은 std::max(DispelTime - SkillDuration, 0);
	float			damage = 0.f;
	float			speed = 0.f;
	float			duration = 0.f;

public:
	virtual void FixedUpdate(float _fixedRate) override;
	virtual void OnClicked() override;

protected:
	class MovementComponent* mv;
	class BitmapComponent* bm;
	class ClickComponent* clickComponent = nullptr;

	// IReflection을(를) 통해 상속됨
	void ReflectionIn() = 0;
	void ReflectionOut() = 0;
};

