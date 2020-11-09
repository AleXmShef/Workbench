#include "wbpch.h"
#include "Renderer.h"
#include "Logger/Logger.h"

namespace Workbench {
	Renderer::Renderer() {
		BIND_EVENT(this, Renderer::OnWindowEventCallback);
	}
	Renderer::~Renderer() { 
		UNBIND_EVENT(this, Renderer::OnWindowEventCallback);
	}

	void Renderer::Init(std::shared_ptr<Window> window) {
		m_window = window;
	}

	void Renderer::OnWindowEventCallback(const Event<Window::Events>* event) {
		switch (GET_EVENT_TYPE(event)) {
		case Window::Events::WindowResizedEvent: {
			OnResize();
		}
		}
	}
}