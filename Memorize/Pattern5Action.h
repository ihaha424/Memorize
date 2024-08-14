#pragma once

#include "D2DGameEngine/Leaf.h"

#include "PatternAction.h"

struct Pattern5Action final : public PatternAction
{
	void Run(float dt) override;

	float speed{ 0.f };
};
