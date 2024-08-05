#pragma once

#include <dinput.h>

constexpr int KeyBoardInput	= 0;	// 0 ~ 257
constexpr int MouseInput	= 258;	// 258 ~ 261
constexpr int PadInput		= 262;	// 262 ~ ?

class IInputDevice
{
public:
	virtual void Update() = 0;
	virtual const BYTE* GetPressData() = 0;
};