#pragma once
#include "IInputDevice.h"

class Mouse : public IInputDevice
{
public:
	Mouse() {};
	virtual ~Mouse() {};

	HRESULT	Initialize(HINSTANCE hInst, HWND hWnd, LPDIRECTINPUT8& LPDInput);

	// IInputDevice을(를) 통해 상속됨
	void				Update() override;
	virtual const BYTE* GetPressData() override;

private:
	static constexpr unsigned char	mouseSize = 0x04;
	DIMOUSESTATE					mouseState;		//rgbButtons | 0: Left, 1: Right, 2: Wheel, 3: 여분
	LPDIRECTINPUTDEVICE8			mouse = nullptr;
	POINT							mousePos;
public:
	static Math::Vector3					curMousePosition;	//x: MouseX, y: MouseY, z: MouseWheel
	static bool						isMouseInClient;
};