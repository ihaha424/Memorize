// WinBase.h
#pragma once

#include "framework.h"

constexpr uint8_t MAX_CSTR_LENGTH = 0xff;

/**
 * @brief An abstract base class of WinBase types.
 *
 * @tparam DerivedWinAppT
 */
template <class DerivedWinAppT>
class WinBase {
public:
	// Static Window Process Callback Function
	static LRESULT CALLBACK WindowProc(
		HWND hwnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam
	) {
		DerivedWinAppT* pDerivedWinApp = nullptr;

		if (uMsg == WM_NCCREATE) {  // Before the creation stage...
			// Retrieve the create struct when received the CREATE message
			CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			// Extract the WinBase pointer from the create struct
			pDerivedWinApp = reinterpret_cast<DerivedWinAppT*>(pCreate->lpCreateParams);
			// Store the WinBase pointer into this window instance
			if (!SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pDerivedWinApp)) {
				OutputDebugString(L"\tBaseWinApp: Setting Window user data failed!\n");
			}
			pDerivedWinApp->_hwnd = hwnd;
		}
		else {  // After the creation stage...
			// Get the WinBase pointer from the window instance
			pDerivedWinApp = reinterpret_cast<DerivedWinAppT*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		}

		if (pDerivedWinApp) return pDerivedWinApp->handleMessage(uMsg, wParam, lParam);
		else                return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

protected:
	HWND _hwnd;
	virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

public:
	WinBase() : _hwnd(nullptr) {}

	HWND GetWindow() const { return _hwnd; }

private:

	wchar_t _className[MAX_CSTR_LENGTH] = { 0, };
	HINSTANCE _hInstance{ GetModuleHandle(NULL) };

	class WindowClassConfigHelper {
		template < class DerivedWinAppT >
		friend class WinBase;

		WNDCLASS _wndClass;
		WinBase* _this;
	public:
		WindowClassConfigHelper(WinBase* _this)
			: _wndClass{ 0 }, _this{ _this } {}

		WindowClassConfigHelper& setStyle(UINT style) {
			_wndClass.style = style;
			return *this;
		}

		WindowClassConfigHelper& setWindowProcessCallback(
			WNDPROC winProc = &DerivedWinAppT::WindowProc
		) {
			_wndClass.lpfnWndProc = winProc;
			return *this;
		}

		WindowClassConfigHelper& setWindowClassStructExtraBytes(int clsExtra) {
			_wndClass.cbClsExtra = clsExtra;
			return *this;
		}

		WindowClassConfigHelper& setWindowInstanceExtraBytes(int wndExtra) {
			_wndClass.cbWndExtra = wndExtra;
			return *this;
		}

		WindowClassConfigHelper& setInstanceHandle(HINSTANCE hInstance = GetModuleHandle(NULL)) {
			_this->_hInstance = hInstance;
			_wndClass.hInstance = hInstance;
			return *this;
		}

		WindowClassConfigHelper& setIconeHandle(HICON hIcon) {
			_wndClass.hIcon = hIcon;
			return *this;
		}

		WindowClassConfigHelper& setCursorHandle(HCURSOR hCursor) {
			_wndClass.hCursor = hCursor;
			return *this;
		}

		WindowClassConfigHelper& setBackgroundBrushHandle(HBRUSH hBackgroundBrush) {
			_wndClass.hbrBackground = hBackgroundBrush;
			return *this;
		}

		WindowClassConfigHelper& setMenuName(const wchar_t* menuName) {
			_wndClass.lpszMenuName = menuName;
			return *this;
		}

		WindowClassConfigHelper& setClassName(const wchar_t* className) {
			wcsncpy_s(_this->_className, className, MAX_CSTR_LENGTH);
			_this->_className[MAX_CSTR_LENGTH - 1] = L'\0';  // Ensure null termination
			_wndClass.lpszClassName = _this->_className;
			return *this;
		}

		WinBase& endSetting() {
			return *_this;
		}
	};

	class WindowStyleConfigHelper {
		template < class DerivedWinAppT >
		friend class WinBase;

		DWORD     _dwStyle;
		DWORD     _dwExStyle;
		LPCWSTR   _lpWindowName;
		int       _X;
		int       _Y;
		int       _nWidth;
		int       _nHeight;
		HWND      _hWndParent;
		HMENU     _hMenu;

		WinBase* _this;
	public:
		WindowStyleConfigHelper(WinBase* _this)
			: _dwStyle{ WS_OVERLAPPEDWINDOW },
			_dwExStyle{ 0 },
			_lpWindowName{ nullptr },
			_X{ CW_USEDEFAULT },
			_Y{ CW_USEDEFAULT },
			_nWidth{ CW_USEDEFAULT },
			_nHeight{ CW_USEDEFAULT },
			_hWndParent{ 0 },
			_hMenu{ 0 },
			_this{ _this }
		{}

		WindowStyleConfigHelper& setStyle(DWORD style) {
			_dwStyle = style;
			return *this;
		}

		WindowStyleConfigHelper& setExtendedStyle(DWORD extendedStyle) {
			_dwExStyle = extendedStyle;
			return *this;
		}

		WindowStyleConfigHelper& setWindowTitle(const wchar_t* windowTitle) {
			_lpWindowName = windowTitle;
			return *this;
		}

		WindowStyleConfigHelper& setWindowPosition(int x, int y) {
			_X = x;
			_Y = y;

			return *this;
		}

		WindowStyleConfigHelper& setWindowPositionCenter() {
			int screenWidth = GetSystemMetrics(SM_CXSCREEN);
			int screenHeight = GetSystemMetrics(SM_CYSCREEN);

			return setWindowPosition((screenWidth - _nWidth) / 2, (screenHeight - _nHeight) / 2);
		}

		WindowStyleConfigHelper& setWindowSize(int width, int height) {
			_nWidth = width;
			_nHeight = height;
			return *this;
		}

		WindowStyleConfigHelper& setWindowParent(HWND _parent) {
			_hWndParent = _parent;
			return *this;
		}

		WindowStyleConfigHelper& setMenuHandle(DWORD menuHandle) {
			_hMenu = menuHandle;
			return *this;
		}

		WinBase& endSetting() {
			return *_this;
		}
	};

protected:
	WindowClassConfigHelper winClassConfigHelper{ this };
	WindowStyleConfigHelper winStyleConfigHelper{ this };

public:
	WindowClassConfigHelper& getWindowClassConfigurationHelper() {
		return winClassConfigHelper;
	}

	WindowStyleConfigHelper& getWindowStyleConfigurationHelper() {
		return winStyleConfigHelper;
	}

	BOOL registerWindowClass() {
		ATOM catom = RegisterClass(&winClassConfigHelper._wndClass);
		return (catom ? TRUE : FALSE);
	}

	BOOL createWindow() {
		_hwnd = CreateWindowEx(
			winStyleConfigHelper._dwExStyle,
			_className,
			winStyleConfigHelper._lpWindowName,
			winStyleConfigHelper._dwStyle,
			winStyleConfigHelper._X,
			winStyleConfigHelper._Y,
			winStyleConfigHelper._nWidth,
			winStyleConfigHelper._nHeight,
			winStyleConfigHelper._hWndParent,
			winStyleConfigHelper._hMenu,
			_hInstance,
			this
		);

		return (_hwnd ? TRUE : FALSE);
	}
};



