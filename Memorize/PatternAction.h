#pragma once

#include "D2DGameEngine/Leaf.h"

struct PatternAction final : public Action
{
	// TODO: Parsing?
	// 패턴 자체의 쿨타임
	// 차트에서 Pattern_Cool_Time
	float patternCooldownTime{ 0.f };
	void SetCooldown(float _cooldownTime) {
		patternCooldownTime = _cooldownTime;
	}
	float GetCooldownTime() {
		return patternCooldownTime;
	}

	// 다음 패턴 시전 전까지의 간격
	// 차트에서 Pattern_Time
	float patternInterval{ 0.f };
	void SetPatternInterval(float _patternInterval) {
		patternInterval = _patternInterval;
	}
	float GetPatternInterval() {
		return patternInterval;
	}

};
