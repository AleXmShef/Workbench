#include "wbpch.h"
#include "Engine.h"
#include "Physics/PhysicsSystem.h"
#include "Renderer/RenderSystem.h"

namespace Workbench {

	Engine::Engine(EngineProps pParams) : m_props(pParams) {
		Logger::Init();

#ifndef WB_DEBUG
		FreeConsole();
#endif

		BIND_EVENT(this, Engine::onWindowEventCallback);

		BIND_EVENT_TO_LAMBDA(ECS::Events, 
			{
				switch (event->getType()) {
				case ECS::Events::EntityCreatedEvent: {
					WB_CORE_LOG("Entity created: {}", (static_cast<const ECS::EntityCreatedEvent*>(event))->Entity()->str());
					break;
				}
				case ECS::Events::EntityDestroyedEvent: {
					WB_CORE_LOG("Entity destroyed: {}", (static_cast<const ECS::EntityDestroyedEvent*>(event))->Entity()->str());
					break;
				}
				}
			}
		);

		m_BaseWindow = std::shared_ptr<Window>(WB_CREATE_NATIVE_WINDOW(pParams.windowProps));

		m_World = ECS::GetInstance();

		PushLayer(std::make_shared<RenderSystem>(m_BaseWindow));
		PushLayer(std::make_shared<PhysicsSystem>());

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
			WB_CORE_LOG("Window lost focus, pausing");
			m_onPause = true;
			break;
		}

		case E::WindowGainedFocusEvent :
		{
			WB_CORE_LOG("Window gained focus back, unpausing");
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
				//WB_CORE_LOG("Left mouse button pressed");
				break;
			case WB_KEYCODES::MMB:
				//WB_CORE_LOG("Middle mouse button pressed");
				break;
			case WB_KEYCODES::RMB:
				//WB_CORE_LOG("Right mouse button pressed");
				break;
			}
			break;
		}

		case E::WindowButtonPressedEvent :
		{
			auto key = ((Window::WindowButtonPressedEvent*)event)->getButton();
			//if(key >= WB_KEYCODES::WB_VK_A && key <= WB_KEYCODES::WB_VK_Z)
			//	WB_CORE_LOG("Button pressed: {}", (char)key);
			//else if(key >= WB_KEYCODES::WB_VK_F1 && key <= WB_KEYCODES::WB_VK_F12)
			//	WB_CORE_LOG("Button pressed: F{}", (int)key - ((int)WB_KEYCODES::WB_VK_F1 - 1));
			if (key == WB_KEYCODES::WB_VK_F11)
				m_BaseWindow->ToggleFullscreen();
			break;
		}
		case E::WindowButtonReleasedEvent :
		{
			auto key = ((Window::WindowButtonPressedEvent*)event)->getButton();
			break;
		}
		case E::WindowMouseWheelRotatedEvent:
		{
			auto delta = ((Window::WindowMouseWheelRotatedEvent*)event)->getDelta();
			//WB_CORE_LOG("Mouse wheel rotated: {}", delta);
			break;
		}
		}
	}

	int Engine::Run() {

		m_GameTimer.Start();

		//m_BaseWindow->OnUpdate(&m_GameTimer);
		//FLUSH_EVENTS();

		while (m_mainLoopFlag) {
			m_GameTimer.Tick();
			m_BaseWindow->OnUpdate(&m_GameTimer);

			if (!m_onPause) {
				OnUpdate(&m_GameTimer);
				FLUSH_EVENTS();
			}
			else
				Sleep(100);
		}
		WB_CORE_INFO("Program ended normally.");
		return 0;
	}

	ECS* Engine::GetWorld() { return m_World; };
}
