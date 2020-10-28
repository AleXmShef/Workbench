#pragma once
#include "Window.h"

namespace Workbench {
	class Renderer {
	public:
		virtual void Init(std::shared_ptr<Window> window);
		virtual void Draw() = 0;
		~Renderer();
	protected:
		Renderer();
	protected:
		std::shared_ptr<Window> m_window;
		void onWindowEventCallback(const Window::Event* event);
		virtual void onResize() = 0;
	};
}