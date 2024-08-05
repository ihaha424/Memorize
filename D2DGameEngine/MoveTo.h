#pragma once

#include "Leaf.h"

struct MoveTo final : public Action {
	using KeyName = std::string;

	void SetObserveKey(const KeyName& key) { _key = key; }
	void SetAcceptableRadius(float rad) { _acceptableRadius = rad; }
	float GetAcceptableRadius() { return _acceptableRadius; }

	void Init() override { status = NodeStatus::Ready; }

	bool IsRunning() override;

	void Run(float dt) override;

private:
	KeyName _key;
	float _acceptableRadius{ 1.f };
};
