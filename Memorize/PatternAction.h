#pragma once

#include "D2DGameEngine/Leaf.h"

struct PatternAction : public Action
{
	using Super = PatternAction;
	// TODO: Parsing?
	// �ൿ Ʈ�������� ��Ÿ��
	// ��Ʈ���� Pattern_Cool_Time
	float patternCooldownTime{ 0.f };
	void SetCooldown(float _cooldownTime) {
		patternCooldownTime = _cooldownTime;
	}
	float GetCooldown() {
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

	// �ൿ Ʈ�� ��ü�� ��Ÿ��
	// ��Ʈ���� Pattern_Delay
	float treeCooldown{ 0.f };
	void SetTreeCooldown(float _treeCooldown) {
		treeCooldown = _treeCooldown;
	}
	float GetTreeCooldown() {
		return treeCooldown;
	}

	float elapsedTime{ 0.f };
	bool IsRunning() override;

	void Run(float dt) override {
		elapsedTime += dt;
	}

};
