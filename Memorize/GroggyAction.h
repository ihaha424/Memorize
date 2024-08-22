#pragma once

#include "D2DGameEngine/Leaf.h"

struct GroggyAction final : public Action
{
	float groggyTimer{ 6.f };

	bool IsRunning() override;

	void Run(float dt) override;

};
