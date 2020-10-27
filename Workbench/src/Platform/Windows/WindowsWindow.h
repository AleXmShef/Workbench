#pragma once
#include "Window.h"

namespace Workbench {
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(Window::WindowProps* props);

		virtual void OnUpdate() override;
		virtual void OnClose() override;

		virtual std::pair<uint32_t, uint32_t> GetDimensions() const override;

		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

		virtual void* GetNativeWindow() override;
	protected:
		static std::unordered_map<HWND, WindowsWindow*> m_assocForWindowsProc;
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		LRESULT CALLBACK CallBackDelegate(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	protected:
		HINSTANCE m_hInstance = NULL;
		WNDCLASS m_wndClass;
		HWND m_hWnd;

		bool _wasResizing = false;

		std::unique_ptr<WindowProps> m_props;
	};
}

