#pragma once

#include "D2DGameEngine\Leaf.h"

struct TeleportTo final : public Action {
	using Super = Action;
	using KeyName = std::string;

	void Init() override;

	/**
	 * @brief Observe the teleport location key.
	 * @param _key 
	 */
	void SetObserveLocationKey(const KeyName& _key) { key = _key; }

	bool IsRunning() override;

	void Run(float dt) override;

private:
	KeyName key;

	bool channeling{ false };
	float castingTime{ .8f };
	bool cooling{ false };
	float coolingTime{ .25f };
	bool done{ false };

	void Teleport();
};

