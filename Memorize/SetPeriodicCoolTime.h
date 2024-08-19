#pragma once

#include "common.h"

#include "D2DGameEngine/Leaf.h"

struct SetPeriodicCoolTime final : public Action {
	float coolTime;
	SetPeriodicCoolTime(float _coolTime) : coolTime{ _coolTime } {}

	bool IsRunning() override;
	void Run(float dt) override;
};
