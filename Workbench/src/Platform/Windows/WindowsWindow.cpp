#include "wbpch.h"
#include "Logger.h"
#include "Events/Event.h"

#include "WindowsWindow.h"
#include "WindowsUtility.h"

namespace Workbench {
	std::unordered_map<HWND, WindowsWindow*>WindowsWindow::m_assocForWindowsProc = {};

	WindowsWindow::WindowsWindow(Window::WindowProps* props) : m_props(props) {
		//get Windows module handle
		m_hInstance = GetModuleHandle(NULL);

		//register Windows window class
		m_wndClass = {};
		m_wndClass.lpfnWndProc = WindowsWindow::WindowProc;
		m_wndClass.hInstance = m_hInstance;
		m_wndClass.lpszClassName = L"IDK";

		RegisterClass(&m_wndClass);

		//create Windows window
		m_hWnd = CreateWindowExW(
			0,
			L"IDK",
			s2ws(m_props->windowTitle).c_str(),
			m_props->isFullScreen ? WS_POPUP : WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			m_props->isFullScreen ? GetSystemMetrics(SM_CXSCREEN) : m_props->windowWidth,
			m_props->isFullScreen ? GetSystemMetrics(SM_CYSCREEN) : m_props->windowHeight,
			NULL,
			NULL,
			m_hInstance,
			NULL
		);

		//show the window
		if (m_hWnd) {
			ShowWindow(m_hWnd, SW_SHOW);
			m_assocForWindowsProc.insert({ m_hWnd, this });
			POST_EVENT(new WindowCreatedEvent(this));
		}
		else
			WB_CORE_ERROR("Failed to create window, failed HWND: {0}", m_hWnd);
	}

	void WindowsWindow::OnUpdate() {
		//process messages from Windows
		MSG msg = {};
		while (PeekMessageA(&msg, m_hWnd, 0, 0, PM_REMOVE)) {
			//GetMessage(&msg, m_hWnd, 0, 0);
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	void WindowsWindow::OnClose()
	{
		m_assocForWindowsProc.erase(m_hWnd);
	}

	LRESULT CALLBACK WindowsWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (!m_assocForWindowsProc.empty())
			return m_assocForWindowsProc[hwnd]->CallBackDelegate(hwnd, uMsg, wParam, lParam);
		else
			return DefWindowProcW(hwnd, uMsg, wParam, lParam);
	}

	LRESULT CALLBACK WindowsWindow::CallBackDelegate(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
		case WM_DESTROY: {
			OnClose();
			POST_EVENT(new WindowDestroyedEvent(this));
			return 0;
			break;
		}
		case WM_ENTERSIZEMOVE: {
			POST_EVENT(new WindowBeganResizeEvent());
			return 0;
			break;
		}
		case WM_EXITSIZEMOVE: {
			if (_wasResizing) {
				SEND_EVENT(new WindowResizedEvent(m_props->windowWidth, m_props->windowHeight));
				_wasResizing = false;
			}
			return 0;
			break;
		}
		case WM_SIZE: {
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
			m_props->windowHeight = height;
			m_props->windowWidth = width;
			_wasResizing = true;
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
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(m_hWnd, &ps);

			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_GRAYTEXT + 1));
			EndPaint(m_hWnd, &ps);
			return 0;
			break;
		}
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		//Event delegation here
	}

	std::pair<uint32_t, uint32_t> WindowsWindow::GetDimensions() const {
		return { m_props->windowWidth, m_props->windowHeight };
	}

	bool WindowsWindow::IsVSync() const {
		return m_props->isVsync;
	}

	void WindowsWindow::SetVSync(bool enabled) {
		m_props->isVsync = enabled;
	}

	void* WindowsWindow::GetNativeWindow() {
		return (void*)m_hInstance;
	}
}