#include "wbpch.h"
#include "Logger/Logger.h"
#include "Events/EventBus.h"
#include "Utils/Utils.h"

#include "WindowsWindow.h"
#include "WindowsUtility.h"

namespace Workbench {

	std::unordered_map<HWND, WindowsWindow*>WindowsWindow::m_assocForWindowsProc = {};

	WindowsWindow::WindowsWindow(Window::WindowProps props) : m_props(props) {
		//get Windows module handle
		m_hInstance = GetModuleHandle(NULL);

		//register Windows window class
		m_wndClass = {};
		m_wndClass.lpfnWndProc = WindowsWindow::WindowProc;
		m_wndClass.hInstance = m_hInstance;
		m_wndClass.lpszClassName = L"IDK";
		m_wndClass.hIcon = LoadIcon(0, IDI_APPLICATION);
		m_wndClass.hCursor = LoadCursor(0, IDC_ARROW);
		
		RegisterClass(&m_wndClass);

		RECT windowRect = { 0, 0, static_cast<LONG>(m_props.windowWidth), static_cast<LONG>(m_props.windowHeight) };
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

		//create Windows window
		m_hWnd = CreateWindowExW(
			0,
			L"IDK",
			s2ws(m_props.windowTitle).c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			NULL,
			NULL,
			m_hInstance,
			NULL
		);

		//show the window
		if (m_hWnd) {
			ShowWindow(m_hWnd, SW_SHOW);
			m_assocForWindowsProc.insert({ m_hWnd, this });
			if (m_props.isFullScreen)
				ToggleFullscreen();
			POST_EVENT(new WindowCreatedEvent(this));
		}
		else
			WB_CORE_ERROR("Failed to create window, failed HWND: {}", m_hWnd);
	}

	bool WindowsWindow::checkForButtonPress(WB_KEYCODES button) {
		auto state = GetKeyState(WbToWinKeycode(button));
		return state < 0;
	}

	void WindowsWindow::OnUpdate(WB_GAME_TIMER* timer) {

//#ifdef WB_DEBUG
		auto mspf = timer->GetMsPerFrame();
		float fps = timer->GetFps();

		std::wstring windowText = s2ws(m_props.windowTitle);
		windowText +=
			L"	 fps: " + std::to_wstring(fps) +
			L", mspf: " + std::to_wstring(mspf);

		SetWindowText(m_hWnd, windowText.c_str());
//#endif

		//process messages from Windows
		MSG msg = {};
		while (PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE)) {
			//GetMessage(&msg, m_hWnd, 0, 0);					//Will be possible when window class will have its on thread
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void WindowsWindow::OnClose()
	{
		m_assocForWindowsProc.erase(m_hWnd);
		DestroyWindow(m_hWnd);
		UnregisterClassW(m_wndClass.lpszClassName, m_hInstance);
	}

	LRESULT CALLBACK WindowsWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (!m_assocForWindowsProc.empty())
			return m_assocForWindowsProc[hwnd]->CallBackDelegate(hwnd, uMsg, wParam, lParam);
		else
			return DefWindowProcW(hwnd, uMsg, wParam, lParam);
	}

	LRESULT CALLBACK WindowsWindow::CallBackDelegate(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
		case WM_ACTIVATE: {
			return 0;
			break;
		}
		case WM_DESTROY: {
			OnClose();
			POST_EVENT(new WindowDestroyedEvent(this));
			return 0;
			break;
		}
		case WM_ENTERSIZEMOVE: {
			POST_EVENT(new WindowBeganResizeEvent());
			_wasResizing = true;
			return 0;
			break;
		}
		case WM_EXITSIZEMOVE: {
			if (_wasResizing) {
				SEND_EVENT(new WindowResizedEvent(m_props.windowWidth, m_props.windowHeight));
				_wasResizing = false;
			}
			return 0;
			break;
		}
		case WM_SIZE: {
			auto __width = LOWORD(lParam);
			auto __height = HIWORD(lParam);

			switch (wParam) {
			case SIZE_MINIMIZED: {
				SEND_EVENT(new WindowLostFocusEvent());
				return 0;
				break;
			}
			default:
			case SIZE_MAXIMIZED:
			case SIZE_RESTORED: {
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				if (width != m_props.windowWidth || height != m_props.windowHeight) {
					m_props.windowWidth = width;
					m_props.windowHeight = height;
					if (!_wasResizing)
						SEND_EVENT(new WindowResizedEvent(m_props.windowWidth, m_props.windowHeight));
				}
				else {
					SEND_EVENT(new WindowGainedFocusEvent());
					return 0;
					break;
				}
			}
			}
			return 0;
			break;
		}
		case WM_GETMINMAXINFO: {
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
			return 0;
			break;
		}
		case WM_MENUCHAR: {
			// Don't beep when we alt-enter.
			return MAKELRESULT(0, MNC_CLOSE);
			break;
		}
		case WM_KEYDOWN: {
			if (!CHECK_BIT(lParam, 30)) {
				POST_EVENT(new WindowButtonPressedEvent(WinToWbKeycode(wParam)));
			}
			return 0;
			break;
		}
		case WM_MOUSEMOVE: {
			UINT x = GET_X_LPARAM(lParam);
			UINT y = GET_Y_LPARAM(lParam);
			POST_EVENT(new WindowMouseMovedEvent(x, y));
			return 0;
			break;
		}
		case WM_LBUTTONDOWN:
			POST_EVENT(new WindowMouseButtonPressedEvent(WinToWbKeycode(VK_LBUTTON)));
			return 0;
			break;
		case WM_MBUTTONDOWN:
			POST_EVENT(new WindowMouseButtonPressedEvent(WinToWbKeycode(VK_MBUTTON)));
			return 0;
			break;
		case WM_RBUTTONDOWN:
			POST_EVENT(new WindowMouseButtonPressedEvent(WinToWbKeycode(VK_RBUTTON)));
			return 0;
			break;
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			return 0;
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	std::pair<uint32_t, uint32_t> WindowsWindow::GetDimensions() const {
		return { m_props.windowWidth, m_props.windowHeight };
	}

	bool WindowsWindow::IsVSync() const {
		return m_props.isVsync;
	}

	void WindowsWindow::SetVSync(bool enabled) {
		m_props.isVsync = enabled;
	}

	void WindowsWindow::ToggleFullscreen() {
		DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW) {
			m_wndStyle = dwStyle;
			GetWindowRect(m_hWnd, &m_wndRect);
			if (GetWindowPlacement(m_hWnd, &m_wpPrev)) {
				SetWindowLong(m_hWnd, GWL_STYLE,
					dwStyle & ~WS_OVERLAPPEDWINDOW);
				ShowWindow(m_hWnd, SW_MAXIMIZE);
				m_props.isFullScreen = true;
			}
		}
		else {
			SetWindowLong(m_hWnd, GWL_STYLE,
				m_wndStyle);
			SetWindowPos(
				m_hWnd, 
				HWND_NOTOPMOST, 
				m_wndRect.left, m_wndRect.top,
				m_wndRect.right - m_wndRect.left,
				m_wndRect.bottom - m_wndRect.top,
				SWP_FRAMECHANGED | SWP_NOACTIVATE
			);
			//AdjustWindowRect(&m_wndRect, WS_OVERLAPPEDWINDOW, FALSE);
			ShowWindow(m_hWnd, SW_NORMAL);
			m_props.isFullScreen = false;
		}
	};

	bool WindowsWindow::IsFullscreen() const {
		return m_props.isFullScreen;
	}

	void* WindowsWindow::GetNativeWindow() {
		return (void*)m_hInstance;
	}

	HWND WindowsWindow::GetHWND() {
		return m_hWnd;
	}
}