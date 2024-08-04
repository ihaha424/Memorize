#pragma once

#include "framework.h"

class GameEngine 
{
public:
	AppWindow hWindow;
	bool stop_rendering{ false };

	class D2DEngine* _d2d{ nullptr };
	
	virtual void Initialize();
	virtual void Execute();
	virtual void Shutdown();

protected:
	virtual void FixedUpdate(float) = 0;
	virtual void Update(float) = 0;
	virtual void Render() = 0;

private:
	void Run();
};
