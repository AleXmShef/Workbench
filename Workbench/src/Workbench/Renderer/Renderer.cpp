#include "wbpch.h"
#include "Renderer.h"
#include "Logger.h"

namespace Workbench {
	Renderer::Renderer() {
		BIND_EVENT(this, Renderer::onWindowEventCallback);
	}
	Renderer::~Renderer() { 
		
	}

	void Renderer::Init(std::shared_ptr<Window> window) {
		m_window = window;
	}

	void Renderer::onWindowEventCallback(const Event<Window::Events>* event) {
		switch (GET_EVENT_TYPE(event)) {
		case Window::Events::WindowResizedEvent: {
			onResize();
		}
		}
	}
}