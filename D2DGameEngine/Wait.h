#pragma once

#include "Leaf.h"

#include "Debug.h"

struct Wait final : public Action {

	Wait(float second) :
		second{ second },
		elapsedTime{ 0.f } {}

	void Init() override {
		status = NodeStatus::Ready;
		elapsedTime = 0.f;
	}

	bool IsRunning() override {
		return second > elapsedTime;
	}
	bool IsFailed() override {
		return false;
	}
	void Run(float dt) override {
		elapsedTime += dt;
	}

private:
	float second;
	float elapsedTime;
};
