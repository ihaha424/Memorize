#pragma once

#include "D2DGameEngine/Leaf.h"

struct Pattern5Action final : public Action
{

	bool IsRunning() override;

	void Run(float dt) override;

};
