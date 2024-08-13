#pragma once

#include "D2DGameEngine/Leaf.h"

struct Pattern9Action final : public Action
{

	bool IsRunning() override;

	void Run(float dt) override;

};
