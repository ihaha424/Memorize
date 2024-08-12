#pragma once

#include "Leaf.h"

struct MoveTo final : public Action {
	using KeyName = std::string;

	void SetObserveLocationKey(const KeyName& _key) { key = _key; }
	void SetAcceptableRadius(float rad) { acceptableRadius = rad; }
	float GetAcceptableRadius() { return acceptableRadius; }

	void Init() override { status = NodeStatus::Ready; }

	bool IsRunning() override;

	void Run(float dt) override;

private:
	KeyName key;
	float acceptableRadius{ 1.f };
};
