#pragma once
#include "wbpch.h"
#include "Layers/Layer.h"
#include "Renderer/Renderer.h"

#include "Primitives/MeshResource.h"

namespace Workbench {
	class RenderSystem : public Layer {
	public:
		RenderSystem(std::shared_ptr<Window> window);

		virtual void OnAttach() override;
		virtual void OnUpdate(WB_GAME_TIMER* timer) override;
		virtual void OnDetach() override;

		d3dRenderer* getRenderer() { return static_cast<d3dRenderer*>(m_Renderer); }

	protected:
		Renderer* m_Renderer = nullptr;
		std::shared_ptr<Window> m_Window = nullptr;

		std::vector<MeshResource> m_MeshResources;
	};
}

