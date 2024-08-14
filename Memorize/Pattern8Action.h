#pragma once

#include "D2DGameEngine/Leaf.h"

#include "PatternAction.h"

struct Pattern8Action final : public PatternAction
{
	void Run(float dt) override;

};
