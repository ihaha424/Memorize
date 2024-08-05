#include "framework.h"
#include "InputSystem.h"
#include "KeyBoard.h"
#include "Mouse.h"

HRESULT InputSystem::Initialize(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr = S_OK;

	hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&LPDInput, NULL);

	KeyBoard* keyboard = new KeyBoard();
	if (SUCCEEDED(hr))
		hr = keyboard->Initialize(hInst, hWnd, LPDInput);
	device.push_back(keyboard);

	Mouse* mouse = new Mouse();
	if (SUCCEEDED(hr))
		hr = mouse->Initialize(hInst, hWnd, LPDInput);
	device.push_back(mouse);


	return hr;
}

void InputSystem::Update()
{
	for (auto Device : device)
	{
		Device->Update();
	}
}

void InputSystem::Finalize()
{
	auto iter = device.begin();
	for (; iter!=device.end();)
	{
		auto deleteDevice = iter;
		iter++;
		delete *deleteDevice;
	}
}