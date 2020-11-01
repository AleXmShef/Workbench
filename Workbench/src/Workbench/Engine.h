#pragma once
#pragma warning(disable : 4251)

#include "Core.h"
#include "Logger.h"
#include "Events/Event.h"

#ifdef WB_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#define WB_CREATE_NATIVE_WINDOW(x) new WindowsWindow(x)

#include "Platform/DirectX/d3dRenderer.h"
#endif

namespace Workbench {
	class WORKBENCH_API Engine
	{
	public:
		struct EngineProps {
			bool showWindow = false;
			std::string windowTitle = "Your title here";
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
		void onWindowEventCallback(const Event < Window::Events>* event);
	private:
		bool m_mainLoopFlag = true;
		bool m_onPause = false;

		std::unique_ptr<EngineProps> m_props = nullptr;

		std::shared_ptr<Window> m_BaseWindow = nullptr;
		std::vector<Window*> m_SecondaryWindows;

		std::unique_ptr<Renderer> m_Renderer = nullptr;
	};
}
