#pragma once
#include "Window.h"

namespace Workbench {
	class Renderer {
	public:
		virtual void Init(std::shared_ptr<Window> window);
		virtual void Begin() = 0;
		virtual void Draw() = 0;
		virtual void End() = 0;
		virtual ~Renderer();
	protected:
		Renderer();
	protected:
		std::shared_ptr<Window> m_window;
		void OnWindowEventCallback(const Event<Window::Events>* event);
		virtual void OnResize() = 0;
	};
}


#if defined(WB_PLATFORM_WINDOWS) and defined(WB_BUILD_DLL)
	#include "Platform/DirectX/d3dRenderer.h"
	#define WB_CREATE_RENDERER() new d3dRenderer()
#endif