#pragma once

#include "D2DGameEngine/Leaf.h"

#include "PatternAction.h"

struct Pattern5Action final : public PatternAction
{

	bool IsRunning() override;

	void Run(float dt) override;

};
