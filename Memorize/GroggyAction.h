#pragma once

#include "D2DGameEngine/Leaf.h"

struct GroggyAction final : public Action
{
	float groggyTimer{ 8.f };

	bool IsRunning() override;

	void Run(float dt) override;

};
