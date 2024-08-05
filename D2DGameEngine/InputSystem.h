#pragma once

#include "IInputDevice.h"

class InputSystem
{
public:
	InputSystem() {};
	~InputSystem() {};

	HRESULT Initialize(HINSTANCE hInst, HWND hWnd);
	void Update();
	void Finalize();

private:
	LPDIRECTINPUT8	LPDInput = nullptr;
	std::vector<IInputDevice*>	device = {};

	friend class InputAction;
};