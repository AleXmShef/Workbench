#include "wbpch.h"
#include "Event.h"

namespace Workbench {
	AbstractCallbackHandler::~AbstractCallbackHandler() {};

	void AbstractCallbackHandler::exec(const AbstractEvent* event) {
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

	void EventBus::propagateEvent(AbstractEvent* event, std::type_index type) {
		for (auto callback : m_SubscribersMap[type]) {
			callback->exec(event);
		}
	}

	MainEventBus* MainEventBus::getInstance() { return m_Instance; };

	MainEventBus* MainEventBus::m_Instance = new MainEventBus;
}