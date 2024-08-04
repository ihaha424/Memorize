#pragma once

#include "framework.h"

#include "WinBase.h"



class WinApp : public WinBase<WinApp> {

	WinApp(HINSTANCE _hInstance, const wchar_t* _className) {
		// Register the window class
		this->getWindowClassConfigurationHelper()
			.setClassName(_className)
			.setInstanceHandle(_hInstance)
			.setWindowProcessCallback()
			.setStyle(CS_HREDRAW | CS_VREDRAW)
			.setBackgroundBrushHandle(CreateSolidBrush(RGB(0, 0, 0)))
			.setIconeHandle(LoadIcon(0, IDI_APPLICATION))
			.setCursorHandle(LoadCursor(0, IDC_ARROW))
			.endSetting()
			.registerWindowClass();
	}

public:
	/**
	 * @brief Message handler
	 *
	 * @param uMsg
	 * @param wParam
	 * @param lParam
	 * @return LRESULT
	 */
	LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

public:

	static void App_Init(HINSTANCE _hInstance = GetModuleHandle(NULL), const wchar_t* _className = L"WinClass");
	static AppWindow App_CreateWindow(int _width, int _height, const wchar_t* _title = L"", DWORD _style = WS_OVERLAPPED);
	static void App_Destroy();
};
