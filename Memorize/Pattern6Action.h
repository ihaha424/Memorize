#pragma once

#include "D2DGameEngine/Leaf.h"
#include "D2DGameEngine/Reflection.h"

#include "PatternAction.h"

struct Pattern6Action final : public PatternAction
{
	float	delay;
	int		posCircle;
	float	bossSpeed;
	void Run(float dt) override;
};
