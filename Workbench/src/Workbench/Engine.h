#pragma once
#include "Core.h"
#include "Logger.h"
#include "Events/Event.h"

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
		
		void Run();

	private:
		void onWindowEvent(const Window::WindowEvent* event) {
			WB_CORE_INFO("Window event: {0}", event->getMsg());
		}
	private:
		bool m_isRunning = true;

		std::unique_ptr<EngineProps> m_props = nullptr;
		std::vector<Window*> m_windows;
	};
}
