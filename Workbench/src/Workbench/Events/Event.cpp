#include "wbpch.h"
#include "Event.h"

namespace Workbench {
	Event::Event() {};
	Event::~Event() {};

	HandlerFunctionBase::~HandlerFunctionBase() {};

	void HandlerFunctionBase::exec(const Event* event) {
		call(event);
	}

	

	void EventBus::flush() {
		deleteAllPending();
		while (!m_EventQueue.empty()) {
			auto [type, event] = m_EventQueue.front();
			propagateEvent(event, type);
			m_EventQueue.pop();
		}
	}

	void EventBus::propagateEvent(Event* event, std::type_index type) {
		for (auto consumer_fn : m_EventConsumerMap[type]) {
			consumer_fn->exec(event);
		}
	}

	MainEventBus* MainEventBus::getInstance() { return m_Instance; };

	MainEventBus* MainEventBus::m_Instance = new MainEventBus;
}