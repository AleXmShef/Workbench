#include "wbpch.h"
#include "Engine.h"

namespace Workbench {

	Engine::Engine(EngineProps* pParams) : m_props(pParams) {
		Logger::Init();

		BIND_EVENT(this, Engine::onWindowEventCallback);

		auto windowProps = new Window::WindowProps;
		*windowProps = {
			m_props->windowTitle,
			m_props->windowWidth,
			m_props->windowHeight,
			m_props->isFullScreen,
			m_props->isVsync
		};

		m_BaseWindow = std::unique_ptr<Window>(WB_CREATE_NATIVE_WINDOW(windowProps));

		WB_CORE_INFO("Workbench initialized successfuly, main thread_id: {0}", std::this_thread::get_id());
	}

	Engine::~Engine() {
		m_BaseWindow.reset();
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
			if (((Window::WindowDestroyedEvent*)event)->getWindow() == m_BaseWindow.get()) {
				WB_CORE_LOG("Base window closed, terminating.");
				m_mainLoopFlag = false;
			}
			break;
		}

		case E::WindowBeganResizeEvent :
		{
			//Will be possible when window will have a separate thread for processing messages
			//m_onPause = true;
			break;
		}

		case E::WindowResizedEvent :
		{
			auto [width, height] = ((Window::WindowResizedEvent*)event)->dimensions;
			WB_CORE_LOG("Window resized event: width: {0}, height : {1}", width, height);
			m_onPause = false;
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
		m_Renderer = std::unique_ptr<d3dRenderer>(new d3dRenderer);
		m_Renderer->Init(m_BaseWindow);

		while (m_mainLoopFlag) {
			if (!m_onPause) {
				FLUSH_EVENTS();
				m_BaseWindow->OnUpdate();
				m_Renderer->Draw();
			}
		}
		WB_CORE_INFO("Program ended normally.");
		return 0;
	}
}
