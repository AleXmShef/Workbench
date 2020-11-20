#include "wbpch.h"
#include "RenderSystem.h"

namespace Workbench {

	RenderSystem::RenderSystem(std::shared_ptr<Window> window) {
		m_Renderer = WB_CREATE_RENDERER();
		m_Window = window;
	}

	void RenderSystem::OnAttach() {
		m_Renderer->Init(m_Window);
		auto VertexBuffer = m_Renderer->CreateConstantResource();
		auto IndexBuffer = m_Renderer->CreateConstantResource();
		
		m_MeshResources.emplace_back(VertexBuffer, IndexBuffer);
	}

	void RenderSystem::OnUpdate(WB_GAME_TIMER* timer) {
		m_Renderer->Begin();
		m_Renderer->End();
	}

	void RenderSystem::OnDetach() {
		delete m_Renderer;
	}
}
