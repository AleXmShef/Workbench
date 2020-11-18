#pragma once
#include "wbpch.h"
#include "Events/Event.h"
#include "Events/CallbackHandler.h"

namespace Workbench {
	class WORKBENCH_API EventBus {
	public:
		using SibscribersMap = std::unordered_map<std::type_index, std::list<AbstractCallbackHandler*>>;
		using EventQueue = std::queue<std::pair<std::type_index, AbstractEvent*>>;
		using PendingDeletionList = std::list<std::pair<std::type_index, AbstractCallbackHandler*>>;

		template<class EventConsumer, class EventType>
		void subscribe(EventConsumer* consumer, void (EventConsumer::* callback)(const Event<EventType>*)) {
			m_SubscribersMap[typeid(EventType)].push_back(new MemberCallbackHandler<EventConsumer, EventType>(consumer, callback));
		};

		template<class EventType>
		void subscribe(std::function<void (const Event<EventType>*)> callback) {
			m_SubscribersMap[typeid(EventType)].push_back(new LambdaCallbackHandler<EventType>(callback));
		};

		template<class EventConsumer, class EventType>
		void unsubscribe(EventConsumer* consumer, void (EventConsumer::* callback)(const Event<EventType>*)) {
			AbstractCallbackHandler* comp = new MemberCallbackHandler<EventConsumer, EventType>(consumer, callback);
			m_PendingDeletionList.push_back({ typeid(EventType), comp });
		}

		template <class EventType>
		void sendEvent(const Event<EventType>* event) {
			propagateEvent((AbstractEvent*)event, typeid(EventType));
		}

		template <class EventType>
		void postEvent(const Event<EventType>* event) {
			m_EventQueue.push({ typeid(EventType), (AbstractEvent*)event });
		}

		void flush();
	protected:
		void deleteAllPending() {
			for (auto [type, handler] : m_PendingDeletionList) {
				auto __handler = handler;
				m_SubscribersMap[type].remove_if([__handler](auto _handler) {return *__handler == *_handler; });
			}
		};
		void propagateEvent(AbstractEvent* event, std::type_index type);
	protected:

		PendingDeletionList		m_PendingDeletionList;
		SibscribersMap			m_SubscribersMap;
		EventQueue				m_EventQueue;
	};

	class WORKBENCH_API MainEventBus : public EventBus {
	public:
		static MainEventBus* getInstance();
	protected:
		MainEventBus() {};
		static MainEventBus* m_Instance;
	};
}

