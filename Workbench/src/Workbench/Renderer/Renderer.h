#pragma once
#include "Window.h"
#include "Renderer/Primitives/ConstantResource.h"
#include "Renderer/Primitives/DynamicResource.h"
#include "Renderer/Primitives/MeshResource.h"

namespace Workbench {
	class Renderer {
	public:
		virtual void Init(std::shared_ptr<Window> window);

		virtual void Begin() = 0;
		virtual void Draw() = 0;
		virtual void DrawMeshes(MeshResource* mesh) = 0;
		virtual void End() = 0;

		//virtual MeshResource* CreateMeshResource() = 0;
		virtual ConstantResource* CreateConstantResource() = 0;
		virtual DynamicResource* CreateDynamicResource(bool isConstantBuffer = false) = 0;

		virtual ~Renderer();
	protected:
		Renderer();
	protected:
		std::shared_ptr<Window> m_window;
		void OnWindowEventCallback(const Event<Window::Events>* event);
		virtual void OnResize() = 0;
	};
}


#if defined(WB_PLATFORM_WINDOWS)
	#include "Platform/DirectX/d3dRenderer.h"
	#define WB_CREATE_RENDERER() new d3dRenderer()
#endif
