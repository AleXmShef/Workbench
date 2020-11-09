#include "wbpch.h"
#include "LayerStack.h"

namespace Workbench {
	void LayerStack::PushLayer(std::shared_ptr<Layer> layer) {
		m_LayerStack.emplace(m_LayerStack.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
		m_LayerStack.back()->OnAttach();
	}

	void LayerStack::PushOverlay(std::shared_ptr<Layer> layer) {
		m_LayerStack.emplace_back(layer);
	}

	void LayerStack::PopLayer() {
		auto it = m_LayerStack.begin() + m_LayerInsertIndex - 1;
		m_LayerStack[m_LayerInsertIndex - 1]->OnDetach();
		m_LayerStack.erase(it);
		m_LayerInsertIndex--;
	}

	void LayerStack::PopOverlay() {
		m_LayerStack.back()->OnDetach();
		m_LayerStack.pop_back();
	}

	void LayerStack::OnUpdate(WB_GAME_TIMER* timer) {
		for (int i = (int)m_LayerStack.size() - 1; i >= 0; --i) {
			m_LayerStack[i]->OnUpdate(timer);
		}
	}

	void LayerStack::OnDetach() {
		for (int i = (int)m_LayerStack.size() - 1; i >= 0; --i) {
			m_LayerStack[i]->OnDetach();
			m_LayerStack.pop_back();
		}
	}

	LayerStack::~LayerStack() {
		OnDetach();
	}
}
