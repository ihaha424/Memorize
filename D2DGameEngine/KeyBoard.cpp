#include "framework.h"
#include "KeyBoard.h"
#include "InputAction.h"

KeyBoard::~KeyBoard()
{}

HRESULT KeyBoard::Initialize(HINSTANCE hInst, HWND hWnd, LPDIRECTINPUT8& LPDInput)
{
	HRESULT hr = S_OK;

	hr = LPDInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	if (SUCCEEDED(hr))
		hr = keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	if (SUCCEEDED(hr))
		hr = keyboard->SetDataFormat(&c_dfDIKeyboard);

	if (SUCCEEDED(hr))
		hr = keyboard->Acquire();

	if (SUCCEEDED(hr))
		keyboard->GetDeviceState(256, (LPVOID)curKeyState);
	return hr;
}
void KeyBoard::Update()
{
	HRESULT hr;
	hr = keyboard->GetDeviceState(256, (LPVOID)curKeyState);
	if (FAILED(hr)) {
		// 디바이스가 포커스를 잃었으면 다시 얻기 시도
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {
			keyboard->Acquire();
		}
		return;
	}

	for (unsigned char key = 0; key < keyBoardSize; key++)
	{
		if (curKeyState[key] & 0x80)
			InputAction::GetInstance()->AddInputKey(key, KeyBoardInput);
	}
}

const BYTE* KeyBoard::GetPressData()
{
	return curKeyState;
}
