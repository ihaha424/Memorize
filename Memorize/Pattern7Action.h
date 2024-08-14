#pragma once

#include "D2DGameEngine/Leaf.h"

#include "PatternAction.h"

struct Pattern7Action final : public PatternAction
{
	// TODO: Parsing?

	float Pattern6_Cool_Time{ 0.f };
	void SetCoolTime(float cooltime) {
		Pattern6_Cool_Time = cooltime;
	}
	float GetCoolTime() {
		return Pattern6_Cool_Time;
	}

	bool IsRunning() override;

	void Run(float dt) override;
};
