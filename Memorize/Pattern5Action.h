#pragma once

#include "D2DGameEngine/Leaf.h"

#include "PatternAction.h"

struct Pattern5Action final : public PatternAction
{
	const char* destinationKey = "Pattern5Destination";
	const float acceptableRadius = 100.f;

	bool IsRunning() override;
	void Run(float dt) override;

	float speed{ 0.f };
	float castingInterval{ 0.f };
};
