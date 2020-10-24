#include "wbpch.h"
#include "Engine.h"

namespace Workbench {

	Engine::Engine(EngineProps* pParams) : m_props(pParams) {
		Logger::Init();

		MainEventBus::getInstance()->subscribe(this, &Engine::onWindowEvent);

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
		WB_CORE_LOG("Workbench started, main thread_id: {0}", std::this_thread::get_id());
		while (m_isRunning) {
			for(Window* window : m_windows)
				window->OnUpdate();
		}
	}

}
