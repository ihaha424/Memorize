#pragma once

#include "Leaf.h"

struct MoveTo final : public Action {
	using KeyName = std::string;

	void SetObserveLocationKey(const KeyName& _key) { key = _key; }
	void SetAcceptableRadius(float rad) { acceptableRadius = rad; }
	float GetAcceptableRadius() { return acceptableRadius; }
	void SetSpeed(float _speed) { speed = _speed; }

	bool IsRunning() override;

	void Run(float dt) override;

private:
	KeyName key;
	float acceptableRadius{ 1.f };
	float speed{ 0.f };
};


struct MoveToLocation final : public Action {

	void SetDestination(const Math::Vector2& _destination) {
		destination = _destination;
	}

	void SetAcceptableRadius(float _acceptableRadius) {
		acceptableRadius = _acceptableRadius;
	}

	void SetSpeed(float _speed) {
		speed = _speed;
	}

	bool IsRunning() override;

	void Run(float dt) override;

private:
	Math::Vector2 destination;
	float acceptableRadius;
	float speed;
};
