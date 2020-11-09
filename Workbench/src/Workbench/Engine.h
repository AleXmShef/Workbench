#pragma once
#pragma warning(disable : 4251)

#include "Core.h"
#include "Logger/Logger.h"
#include "Events/EventBus.h"
#include "Layers/LayerStack.h"
#include "ECS/ECS.h"
#include "Time/Timer.h"
#include "Window.h"

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
		void onWindowEventCallback(const Event<Window::Events>* event);
	private:
		bool m_mainLoopFlag = true;
		bool m_onPause = false;

		ECS* m_World = nullptr;

		WB_GAME_TIMER m_GameTimer;

		std::unique_ptr<EngineProps> m_props = nullptr;

		std::shared_ptr<Window> m_BaseWindow = nullptr;

		std::unique_ptr<LayerStack> m_LayerStack = nullptr;
	};
}
