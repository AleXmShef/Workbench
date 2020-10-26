#include "wbpch.h"
#include "Engine.h"

namespace Workbench {

	Engine::Engine(EngineProps* pParams) : m_props(pParams) {
		Logger::Init();

		MainEventBus::getInstance()->subscribe(this, &Engine::onWindowEventCallback);

		auto windowProps = new Window::WindowProps;
		*windowProps = {
			m_props->windowTitle,
			m_props->windowWidth,
			m_props->windowHeight,
			m_props->isFullScreen,
			m_props->isVsync
		};

		m_BaseWindow = WB_CREATE_NATIVE_WINDOW(windowProps);
	}

	Engine::~Engine() {
		delete m_BaseWindow;
	}

	void Engine::onWindowEventCallback(const Window::Event* event) {
		using E = Window::Events;

		switch (GET_EVENT_TYPE(event)) {

		case E::WindowCreatedEvent :
		{
			WB_CORE_LOG("Window created event");
			break;
		}

		case E::WindowDestroyedEvent :
		{
			WB_CORE_LOG("Window destroyed event");
			if (((Window::WindowDestroyedEvent*)event)->getWindow() == m_BaseWindow) {
				WB_CORE_LOG("Base window closed, terminating.");
				m_isRunning = false;
			}
			break;
		}

		case E::WindowResizedEvent :
		{
			auto [width, height] = ((Window::WindowResizedEvent*)event)->dimensions;
			WB_CORE_LOG("Window resized event: width: {0}, height : {1}", width, height);
			break;
		}

		case E::WindowMouseMovedEvent :
		{
			auto [xPos, yPos] = ((Window::WindowMouseMovedEvent*)event)->getCoords();
			WB_CORE_LOG("Window mouse moved event: {0}, {1}", xPos, yPos);
			break;
		}

		}
	}

	int Engine::Run() {
		WB_CORE_INFO("Workbench started, main thread_id: {0}", std::this_thread::get_id());
		while (m_isRunning) {
			FLUSH_EVENTS();
			m_BaseWindow->OnUpdate();
		}
		WB_CORE_INFO("Program ended normally.");
		return 0;
	}
}
