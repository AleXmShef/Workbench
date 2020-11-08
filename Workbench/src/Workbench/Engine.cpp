#include "wbpch.h"
#include "Engine.h"
#include "Physics/PhysicsComponent.h"
#include "Physics/PhysicsSystem.h"

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
	}

	Engine::~Engine() {
		UNBIND(this, Engine::onWindowEventCallback);
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
				m_Renderer.reset();
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
			WB_CORE_LOG("Window resized event: width: {0}, height : {1}", width, height);
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
				WB_CORE_TRACE("Left mouse button pressed");
				break;
			case WB_KEYCODES::MMB:
				WB_CORE_TRACE("Middle mouse button pressed");
				break;
			case WB_KEYCODES::RMB:
				WB_CORE_TRACE("Right mouse button pressed");
				break;
			}
			break;
		}

		case E::WindowButtonPressedEvent :
		{
			auto key = ((Window::WindowButtonPressedEvent*)event)->getButton();
			if(key >= WB_KEYCODES::WB_VK_A && key <= WB_KEYCODES::WB_VK_Z)
				WB_CORE_TRACE("Button pressed: {}", (char)key);
			if (key == WB_KEYCODES::WB_VK_F11)
				m_BaseWindow->ToggleFullscreen();
			break;
		}
		}
	}

	int Engine::Run() {

		m_BaseWindow->OnUpdate();
		FLUSH_EVENTS();

		m_Renderer = std::unique_ptr<d3dRenderer>(new d3dRenderer);
		m_Renderer->Init(m_BaseWindow);

		m_LayerStack->PushLayer(std::make_shared<PhysicsSystem>());

		auto entity1 = m_World->CreateEntity();
		auto entity2 = m_World->CreateEntity();

		auto m_component1 = new PhysicsComponent;
		auto m_component2 = new PhysicsComponent;

		m_component1->data.x = 5;
		m_component2->data.x = 10;

		m_component1->data.acc = 2;
		m_component2->data.acc = 9.8;

		m_World->AddComponent(entity1, m_component1);
		m_World->AddComponent(entity2, m_component2);


		while (m_mainLoopFlag) {
			m_BaseWindow->OnUpdate();
			if (!m_onPause) {
				m_LayerStack->OnUpdate();
				m_Renderer->Draw();
				FLUSH_EVENTS();
			}
			else
				Sleep(5);
		}
		WB_CORE_INFO("Program ended normally.");
		return 0;
	}
}
