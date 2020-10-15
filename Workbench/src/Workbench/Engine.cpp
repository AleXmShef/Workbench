#include "wbpch.h"
#include "Engine.h"

namespace Workbench {

	Engine::Engine(EngineProps* pParams) : m_props(pParams) {
		auto windowProps = new Window::WindowProps;
		*windowProps = {
			m_props->windowTitle,
			m_props->windowWidth,
			m_props->windowHeight,
			m_props->isFullScreen,
			m_props->isVsync
		};

		auto mainWindow = WB_CREATE_NATIVE_WINDOW(windowProps);
		m_windows.push_back(mainWindow);
	}

	Engine::~Engine() {

	}

	void Engine::Run() {
		Logger::Init();
		WB_CORE_LOG("Workbench started {1}, {0}", 1, 5);
		WB_INFO("Log started");
		while (1) {
			m_windows[0]->OnUpdate();
		}
	}

}
