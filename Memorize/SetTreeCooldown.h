#pragma once

#include "common.h"

#include "D2DGameEngine/Leaf.h"

struct SetTreeCoolTime final : public Action {
	float coolTime;
	SetTreeCoolTime(float _coolTime) : coolTime{ _coolTime } {}

	bool IsRunning() override;
	void Run(float dt) override;
};
