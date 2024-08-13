#pragma once

#include "D2DGameEngine/Leaf.h"

struct GroggyAction final : public Action
{

	bool IsRunning() override;

	void Run(float dt) override;

};
