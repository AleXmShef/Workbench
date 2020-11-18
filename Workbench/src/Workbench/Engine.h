#pragma once
#pragma warning(disable : 4251)

#include "Core.h"
#include "Layers/LayerStack.h"
#include "ECS/ECS.h"
#include "Time/Timer.h"
#include "Window.h"

namespace Workbench {
	class WORKBENCH_API Engine : public LayerStack
	{
	public:
		struct EngineProps {
			Window::WindowProps windowProps;
		};

	public:
		Engine(EngineProps pParams);
		~Engine();

		ECS* GetWorld();

		int Run();

	private:
		void onWindowEventCallback(const Event<Window::Events>* event);
	private:
		bool m_mainLoopFlag = true;
		bool m_onPause = false;

		ECS* m_World = nullptr;

		WB_GAME_TIMER m_GameTimer;

		EngineProps m_props;

		std::shared_ptr<Window> m_BaseWindow = nullptr;
	};
}
