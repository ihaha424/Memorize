#pragma once

#include "D2DGameEngine/Leaf.h"
#include "D2DGameEngine/Reflection.h"

#include "PatternAction.h"

struct Pattern6Action final : public PatternAction, IReflection
{
	float	delay;
	int		posCircle;
	float	bossSpeed;
	void Run(float dt) override;

	// IReflection을(를) 통해 상속됨
	void ReflectionIn() override;
	void ReflectionOut() override;
};
