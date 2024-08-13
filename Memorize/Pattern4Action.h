#pragma once

#include "D2DGameEngine/Leaf.h"

struct Pattern4Action final : public Action
{

	bool IsRunning() override;

	void Run(float dt) override;

};
