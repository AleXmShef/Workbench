#pragma once
#include "wbpch.h"
#include "Core.h"
#include "Layer.h"

namespace Workbench {
	class LayerStack : public Layer {
	public:
		//LayerStack();
		virtual ~LayerStack();

		virtual void PushLayer(std::shared_ptr<Layer> layer);
		virtual void PushOverlay(std::shared_ptr<Layer> layer);
		virtual void InsertLayer(std::shared_ptr<Layer> layer, size_t pos) {};
		virtual void InsertOverlay(std::shared_ptr<Layer> layer, size_t pos) {};
		virtual void PopLayer();
		virtual void PopOverlay();
		virtual void RemoveLayer() {};
		virtual void RemoveOverlay() {};

		virtual void OnUpdate(WB_GAME_TIMER* timer) override;
		virtual void OnDetach() override;

		virtual std::shared_ptr<Layer> operator[](const int index);

	protected:
		uint32_t m_LayerInsertIndex = 0;
		std::vector<std::shared_ptr<Layer>> m_LayerStack;
	};
}