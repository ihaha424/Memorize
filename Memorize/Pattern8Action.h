#pragma once

#include "D2DGameEngine/Leaf.h"

struct Pattern8Action final : public Action
{

	bool IsRunning() override;

	void Run(float dt) override;

};
