#pragma once

#include "D2DGameEngine\Leaf.h"

struct TeleportTo final : public Action {
	using KeyName = std::string;

	/**
	 * @brief Observe the teleport location key.
	 * @param _key 
	 */
	void SetObserveLocationKey(const KeyName& _key) { key = _key; }

	bool IsRunning() override;

	void Run(float dt) override;

private:
	KeyName key;
};

