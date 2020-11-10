#include "wbpch.h"
#include "Engine.h"
#include "Physics/PhysicsComponent.h"
#include "Physics/TransformComponent.h"
#include "Physics/PhysicsSystem.h"
#include "Renderer/RenderSystem.h"

namespace Workbench {

	Engine::Engine(EngineProps* pParams) : m_props(pParams) {
		Logger::Init();

#ifndef WB_DEBUG
		FreeConsole();
#endif

		BIND_EVENT(this, Engine::onWindowEventCallback);

		auto _t = new LayerStack();

		//m_LayerStack = (new LayerStack());

		auto windowProps = new Window::WindowProps;
		*windowProps = {
			m_props->windowTitle,
			m_props->windowWidth,
			m_props->windowHeight,
			m_props->isFullScreen,
			m_props->isVsync
		};

		m_BaseWindow = std::shared_ptr<Window>(WB_CREATE_NATIVE_WINDOW(windowProps));

		m_World = ECS::getInstance();

		m_LayerStack = std::make_unique<LayerStack>();

		WB_CORE_INFO("Workbench successfuly initialized, main thread_id: {0}", std::this_thread::get_id());

		WB_CORE_INFO("SIMD Support: {}", (MATHFU_BUILD_OPTIONS_SIMD == "[simd]" ? "yes" : "no"));
	}

	Engine::~Engine() {
		UNBIND_EVENT(this, Engine::onWindowEventCallback);
	}

	void Engine::onWindowEventCallback(const Event<Window::Events>* event) {
		using E = Window::Events;

		switch (event->getType()) {

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
				m_BaseWindow.reset();
				m_mainLoopFlag = false;
			}
			break;
		}

		case E::WindowLostFocusEvent :
		{
			m_onPause = true;
			break;
		}

		case E::WindowGainedFocusEvent :
		{
			m_onPause = false;
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
			//WB_CORE_LOG("Window resized event: width: {0}, height : {1}", width, height);
			m_onPause = false;
			break;
		}

		case E::WindowMouseMovedEvent :
		{
			auto [xPos, yPos] = ((Window::WindowMouseMovedEvent*)event)->getCoords();
			//WB_CORE_LOG("Window mouse moved event: {0}, {1}", xPos, yPos);
			break;
		}

		case E::WindowMouseButtonPressedEvent :
		{
			auto button = ((Window::WindowMouseButtonPressedEvent*)event)->getButton();
			switch (button) {
			case WB_KEYCODES::LMB:
				WB_CORE_LOG("Left mouse button pressed");
				break;
			case WB_KEYCODES::MMB:
				WB_CORE_LOG("Middle mouse button pressed");
				break;
			case WB_KEYCODES::RMB:
				WB_CORE_LOG("Right mouse button pressed");
				break;
			}
			break;
		}

		case E::WindowButtonPressedEvent :
		{
			auto key = ((Window::WindowButtonPressedEvent*)event)->getButton();
			if(key >= WB_KEYCODES::WB_VK_A && key <= WB_KEYCODES::WB_VK_Z)
				WB_CORE_LOG("Button pressed: {}", (char)key);
			if (key == WB_KEYCODES::WB_VK_F11)
				m_BaseWindow->ToggleFullscreen();
			break;
		}
		}
	}

	int Engine::Run() {

		m_GameTimer.Start();

		m_BaseWindow->OnUpdate(&m_GameTimer);
		FLUSH_EVENTS();

		m_LayerStack->PushLayer(std::make_shared<RenderSystem>(m_BaseWindow));
		m_LayerStack->PushLayer(std::make_shared<PhysicsSystem>());

		auto entity1 = m_World->CreateEntity();
		auto entity2 = m_World->CreateEntity();

		auto m_component1 = new TransformComponent(entity1);
		auto m_component2 = new TransformComponent(entity2);

		auto m_component3 = new PhysicsComponent(entity1, 10, mathfu::vec4(0, 0, 0, 0), mathfu::vec4(0, 0, 0, 0));
		auto m_component4 = new PhysicsComponent(entity2, 10, mathfu::vec4(0, 0, 0, 0), mathfu::vec4(0, 0, 0, 0));

		m_World->AddComponent(entity1, m_component1);
		m_World->AddComponent(entity2, m_component2);
		m_World->AddComponent(entity1, m_component3);
		m_World->AddComponent(entity2, m_component4);

		while (m_mainLoopFlag) {
			m_GameTimer.Tick();
			m_BaseWindow->OnUpdate(&m_GameTimer);

			if (!m_onPause) {
				m_LayerStack->OnUpdate(&m_GameTimer);
				FLUSH_EVENTS();
			}
			else
				Sleep(5);
		}
		WB_CORE_INFO("Program ended normally.");
		return 0;
	}
}
