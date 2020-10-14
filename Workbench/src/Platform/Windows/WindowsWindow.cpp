#include "wbpch.h"
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
		m_wndClass.lpszClassName = L"Workbench Window Class";

		RegisterClass(&m_wndClass);

		//create Windows window
		m_hWnd = CreateWindowExW(
			0,
			L"Workbench Window Class",
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

		if (m_hWnd == NULL) {
			//print error
		}

		//show the window
		ShowWindow(m_hWnd, 1);

		m_assocForWindowsProc.insert({ m_hWnd, this });
	}

	void WindowsWindow::OnUpdate() {
		MSG msg = {};
		GetMessage(&msg, m_hWnd, 0, 0);
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	LRESULT CALLBACK WindowsWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		m_assocForWindowsProc[hwnd]->CallBackDelegate(hwnd, uMsg, wParam, lParam);
		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
	}

	void WindowsWindow::CallBackDelegate(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
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