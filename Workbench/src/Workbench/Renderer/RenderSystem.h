#pragma once
#include "wbpch.h"
#include "Layers/Layer.h"
#include "Renderer/Renderer.h"

namespace Workbench {
	class RenderSystem : public Layer {
	public:
		RenderSystem(std::shared_ptr<Window> window);

		virtual void OnAttach() override;
		virtual void OnUpdate(WB_GAME_TIMER* timer) override;
		virtual void OnDetach() override;

	protected:
		Renderer* m_Renderer = nullptr;
		std::shared_ptr<Window> m_Window = nullptr;
	};
}

