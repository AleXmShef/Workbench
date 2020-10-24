#include "wbpch.h"
#include "Logger.h"

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
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			m_props->windowWidth,
			m_props->windowHeight,
			NULL,
			NULL,
			m_hInstance,
			NULL
		);

		//show the window
		if (m_hWnd) {
			ShowWindow(m_hWnd, 1);
			m_assocForWindowsProc.insert({m_hWnd, this});
			MainEventBus::getInstance()->sendEvent(new WindowEvent("test"));
		}
		else
			WB_CORE_ERROR("Failed to create window, failed HWND: {0}", m_hWnd);
	}

	void WindowsWindow::OnUpdate() {
		//process messages from Windows
		MSG msg = {};
		GetMessage(&msg, m_hWnd, 0, 0);
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	void WindowsWindow::OnClose()
	{
		m_assocForWindowsProc.erase(m_hWnd);
	}

	LRESULT CALLBACK WindowsWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (uMsg == WM_DESTROY) {
			WB_CORE_TRACE("Closing window: {0}, thread_id: {1}", hwnd, std::this_thread::get_id());
		}
		if(!m_assocForWindowsProc.empty())
			m_assocForWindowsProc[hwnd]->CallBackDelegate(hwnd, uMsg, wParam, lParam);
		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
	}

	void WindowsWindow::CallBackDelegate(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
		case WM_DESTROY: {
			OnClose();
			break;
		}
		case WM_SIZE: {
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
		}
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