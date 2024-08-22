#include "WinApp.h"

static WinApp* winApp{ nullptr };

LRESULT WinApp::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		// Close the window when user alt-f4s or clicks the X button
	case WM_CLOSE:
		OutputDebugString(L"\WinApp: Destroying this window...\n");
		DestroyWindow(GetWindow());
		return 0;
	case WM_DESTROY:
		OutputDebugString(L"\WinApp: Post Quit Message...\n");
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(_hwnd, uMsg, wParam, lParam);
}

void WinApp::App_Init(HINSTANCE _hInstance, const wchar_t* _className) {
	if (winApp) return;

	winApp = new WinApp(_hInstance, _className);
}

AppWindow WinApp::App_CreateWindow(int _width, int _height, const wchar_t* _title, DWORD _style)
{
	winApp->getWindowStyleConfigurationHelper()
		.setWindowTitle(_title)
		.setStyle(_style)
		.setWindowSize(_width, _height)
		.setWindowPositionCenter()
		.endSetting()
		.createWindow();

	ShowWindow(winApp->GetWindow(), SW_MAXIMIZE);
	return winApp->GetWindow();
}

void WinApp::App_Destroy() {
	if (!winApp) return;

	winApp = nullptr;
}
