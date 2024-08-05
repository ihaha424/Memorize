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


//#define DIK_ESCAPE          0x01
//#define DIK_1               0x02
//#define DIK_2               0x03
//#define DIK_3               0x04
//#define DIK_4               0x05
//#define DIK_5               0x06
//#define DIK_6               0x07
//#define DIK_7               0x08
//#define DIK_8               0x09
//#define DIK_9               0x0A
//#define DIK_0               0x0B
//#define DIK_MINUS           0x0C    /* - on main keyboard */
//#define DIK_EQUALS          0x0D
//#define DIK_BACK            0x0E    /* backspace */
//#define DIK_TAB             0x0F
//#define DIK_Q               0x10
//#define DIK_W               0x11
//#define DIK_E               0x12
//#define DIK_R               0x13
//#define DIK_T               0x14
//#define DIK_Y               0x15
//#define DIK_U               0x16
//#define DIK_I               0x17
//#define DIK_O               0x18
//#define DIK_P               0x19
//#define DIK_LBRACKET        0x1A
//#define DIK_RBRACKET        0x1B
//#define DIK_RETURN          0x1C    /* Enter on main keyboard */
//#define DIK_LCONTROL        0x1D
//#define DIK_A               0x1E
//#define DIK_S               0x1F
//#define DIK_D               0x20
//#define DIK_F               0x21
//#define DIK_G               0x22
//#define DIK_H               0x23
//#define DIK_J               0x24
//#define DIK_K               0x25
//#define DIK_L               0x26
//#define DIK_SEMICOLON       0x27
//#define DIK_APOSTROPHE      0x28
//#define DIK_GRAVE           0x29    /* accent grave */
//#define DIK_LSHIFT          0x2A
//#define DIK_BACKSLASH       0x2B
//#define DIK_Z               0x2C
//#define DIK_X               0x2D
//#define DIK_C               0x2E
//#define DIK_V               0x2F
//#define DIK_B               0x30
//#define DIK_N               0x31
//#define DIK_M               0x32
//#define DIK_COMMA           0x33
//#define DIK_PERIOD          0x34    /* . on main keyboard */
//#define DIK_SLASH           0x35    /* / on main keyboard */
//#define DIK_RSHIFT          0x36
//#define DIK_MULTIPLY        0x37    /* * on numeric keypad */
//#define DIK_LMENU           0x38    /* left Alt */
//#define DIK_SPACE           0x39
//#define DIK_CAPITAL         0x3A
//#define DIK_F1              0x3B
//#define DIK_F2              0x3C
//#define DIK_F3              0x3D
//#define DIK_F4              0x3E
//#define DIK_F5              0x3F
//#define DIK_F6              0x40
//#define DIK_F7              0x41
//#define DIK_F8              0x42
//#define DIK_F9              0x43
//#define DIK_F10             0x44