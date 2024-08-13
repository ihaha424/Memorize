#pragma once
#include "../D2DGameEngine/Actor.h"

class BossSkillActor : public Actor
{
	enum BossSkillType {Projectile, Range};

public:
	BossSkillActor(class World* _world);
	virtual ~BossSkillActor();

public:
	bool			isDispel = false;
	bool			isFragile = false;
	BossSkillType	type = Projectile;
	float			dispelTime = 0.f;
	float			skillDuration = 0.f;
	// 스킬 시전 타이밍은 std::max(DispelTime - SkillDuration, 0);
	float			damage = 0.f;
	float			speed = 0.f;
	float			duration = 0.f;

protected:
	class MovementComponent* mv;
	class BitmapComponent* bm;
};

