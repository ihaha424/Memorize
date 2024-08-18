#pragma once
#include "Projectile.h"

class MeteorEffect : public Projectile
{

	Math::Vector2 attackPos;
public:
	MeteorEffect(World* _world);
	virtual ~MeteorEffect();

	virtual void BeginPlay() override;
	virtual void OnBeginOverlap(Actor* other, const OverlapInfo& overlap) override;
	virtual void Update(float _dt) override;

	void Initialize();
	void SetAttackPos(Math::Vector2 _attackPos) { attackPos = _attackPos; }
};

