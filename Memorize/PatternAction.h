#pragma once

#include "D2DGameEngine/Leaf.h"

struct PatternAction final : public Action
{
	// TODO: Parsing?
	// ���� ��ü�� ��Ÿ��
	// ��Ʈ���� Pattern_Cool_Time
	float patternCooldownTime{ 0.f };
	void SetCooldown(float _cooldownTime) {
		patternCooldownTime = _cooldownTime;
	}
	float GetCooldownTime() {
		return patternCooldownTime;
	}

	// ���� ���� ���� �������� ����
	// ��Ʈ���� Pattern_Time
	float patternInterval{ 0.f };
	void SetPatternInterval(float _patternInterval) {
		patternInterval = _patternInterval;
	}
	float GetPatternInterval() {
		return patternInterval;
	}

};
