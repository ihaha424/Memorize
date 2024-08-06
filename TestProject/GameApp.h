#pragma once

#include "common.h"

#include "../D2DGameEngine/GameEngine.h"

class GameApp : public GameEngine
{
	using Super = GameEngine;

#ifndef NDEBUG 
	float frameTime{ 0.f };
	std::size_t fps{ 0 };
	std::size_t count{ 0 };
#endif

public:

	bool isInitialized{ false };

	void Initialize();
	void Execute();
	void Shutdown();

private:
	void FixedUpdate(float) override;
	void Update(float) override;
	void Render() override;

private:
	class World* world;
};
