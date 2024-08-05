#include "framework.h"
#include "Mouse.h"
#include "InputAction.h"

//Vector3 Mouse::curMousePosition = {};
bool	Mouse::isMouseInClient = false;

HRESULT Mouse::Initialize(HINSTANCE hInst, HWND hWnd, LPDIRECTINPUT8& LPDInput)
{
	HRESULT hr = S_OK;

	hr = LPDInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	if (SUCCEEDED(hr))
		hr = mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	if (SUCCEEDED(hr))
		hr = mouse->SetDataFormat(&c_dfDIMouse);

	if (SUCCEEDED(hr))
		hr = mouse->Acquire();

	if (SUCCEEDED(hr))
		mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);

	SetCursorPos(0, 0);
	GetCursorPos(&mousePos);
	ScreenToClient(hWnd, &mousePos);
	SetCursorPos(-mousePos.x, -mousePos.y);

	return hr;
}
void Mouse::Update()
{
	HRESULT hr;
	hr = mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
	if (FAILED(hr)) {
		// 디바이스가 포커스를 잃었으면 다시 얻기 시도
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {
			mouse->Acquire();
		}
		return;
	}
	for (unsigned char key = 0; key < mouseSize; key++)
	{
		if (mouseState.rgbButtons[key] & 0x80)
			InputAction::GetInstance()->AddInputKey(key, MouseInput);
	}
	//curMousePosition += Vector3(mouseState.lX, mouseState.lY, mouseState.lZ);
	//if (isMouseInClient)
	//{
	//	Vector2 clamp = Screen::GetInstance()->GetSize();
	//	if (0 > curMousePosition.x)
	//		curMousePosition.x = 0;
	//	else if (clamp.x < curMousePosition.x)
	//		curMousePosition.x = clamp.x;
	//	else if (0 > curMousePosition.y)
	//		curMousePosition.y = 0;
	//	else if (clamp.y < curMousePosition.y)
	//		curMousePosition.y = clamp.y;
	//}
	//MouseComponent::GetInstance()->SetPosition(curMousePosition);
	SetCursorPos(-mousePos.x + curMousePosition.x, -mousePos.y + curMousePosition.y);
}

const BYTE* Mouse::GetPressData()
{
	return mouseState.rgbButtons;
}