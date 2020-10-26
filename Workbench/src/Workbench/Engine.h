#pragma once
#pragma warning(disable : 4251)
#include "Core.h"
#include "Logger.h"
#include "Events/Event.h"

#ifdef WB_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
	#define WB_CREATE_NATIVE_WINDOW(x) new WindowsWindow(x)
#endif

namespace Workbench {
	class WORKBENCH_API Engine
	{
	public:
		struct EngineProps {
			bool showWindow = false;
			std::string windowTitle = "Workbench Sandbox";
			uint32_t windowWidth = 1280;
			uint32_t windowHeight = 720;
			bool isFullScreen = false;
			bool isVsync = true;
		};

	public:
		Engine(EngineProps* pParams);
		~Engine();
		
		int Run();

	private:
		void onWindowEventCallback(const Window::Event* event);
	private:
		bool m_isRunning = true;

		std::unique_ptr<EngineProps> m_props = nullptr;
		Window* m_BaseWindow = nullptr;
		std::vector<Window*> m_SecondaryWindows;
	};
}
