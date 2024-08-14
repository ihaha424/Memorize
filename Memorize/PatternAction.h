#pragma once

#include "D2DGameEngine/Leaf.h"

struct PatternAction : public Action
{
	using Super = PatternAction;
	// TODO: Parsing?
	// 행동 트리에서의 쿨타임
	// 차트에서 Pattern_Cool_Time
	float patternCooldownTime{ 0.f };
	void SetCooldown(float _cooldownTime) {
		patternCooldownTime = _cooldownTime;
	}
	float GetCooldown() {
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

	float elapsedTime{ 0.f };
	bool IsRunning() override {
		if (elapsedTime >= patternInterval) 
		{
			elapsedTime = 0.f;
			return false;
		}
		else 
		{
			return true;
		}
	}

	void Run(float dt) override {
		elapsedTime += dt;
	}

};
