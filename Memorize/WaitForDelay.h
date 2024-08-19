#pragma once

#include "common.h"

#include "D2DGameEngine/Leaf.h"

struct WaitForDelay final : public Action {

	WaitForDelay() {}

	bool IsRunning() override;
	void Run(float dt) override;
};
