#pragma once
#include "wbpch.h"
#include "Core.h"
#include "Layer.h"

namespace Workbench {
	class LayerStack : public Layer {
	public:
		//LayerStack();

		virtual void PushLayer(std::shared_ptr<Layer> layer);
		virtual void PushOverlay(std::shared_ptr<Layer> layer);
		virtual void PopLayer();
		virtual void PopOverlay();

		virtual void OnUpdate() override;
		virtual void OnDetach() override;

	protected:
		uint32_t m_LayerInsertIndex = 0;
		std::vector<std::shared_ptr<Layer>> m_LayerStack;
	};
}