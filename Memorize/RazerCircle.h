#pragma once

#include "BossSkillActor.h"

class RazerCircle : public BossSkillActor
{

public:
	RazerCircle(class World* _world);
	virtual ~RazerCircle() {};

	virtual void FixedUpdate(float _fixedRate) override;

	virtual void Update(float _dt) override;

};