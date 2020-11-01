#pragma once
#include "wbpch.h"

namespace Workbench {
	class AbstractEvent {
	};

	template<class EventType>
	class Event : public AbstractEvent {
	public:
		virtual EventType getType() const = 0;
	};

	class AbstractCallbackHandler {
	public:
		virtual ~AbstractCallbackHandler();

		void exec(const AbstractEvent* event);

		virtual bool operator==(const AbstractCallbackHandler& right) = 0;

	protected:
		virtual void call(const AbstractEvent* event) = 0;
	};

	template<class Subscriber, class EventType>
	class MemberCallbackHandler : public AbstractCallbackHandler {
	public:
		typedef void (Subscriber::*Callback)(const Event<EventType>*);

		MemberCallbackHandler(Subscriber* consumer_instance, Callback member_function)
			: m_subscriber(consumer_instance), m_callback(member_function) {};

		virtual bool operator==(const AbstractCallbackHandler& another) {
			auto _another = static_cast<const MemberCallbackHandler<Subscriber, EventType>*>(&another);

			if (_another != nullptr) {
				if (m_subscriber == _another->m_subscriber && m_callback == _another->m_callback)
					return true;
			}

			return false;
		};
	protected:
		virtual void call(const AbstractEvent* event) override {
			(m_subscriber->*m_callback)(static_cast<const Event<EventType>*>(event));
		}

	protected:
		Subscriber*		m_subscriber;
		Callback		m_callback;
	};

	class EventBus {
	public:
		using SibscribersMap =			std::unordered_map<std::type_index, std::list<AbstractCallbackHandler*>>;
		using EventQueue =				std::queue<std::pair<std::type_index, AbstractEvent*>>;
		using PendingDeletionList =		std::list<std::pair<std::type_index, AbstractCallbackHandler*>>;

		template<class EventConsumer, class EventType>
		void subscribe(EventConsumer* consumer, void (EventConsumer::* callback)(const Event<EventType>*)) {
			m_SubscribersMap[typeid(EventType)].push_back(new MemberCallbackHandler<EventConsumer, EventType>(consumer, callback));
		};

		template<class EventConsumer, class EventType>
		void unsubscribe(EventConsumer* consumer, void (EventConsumer::*callback)(const Event<EventType>*)) {
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

	class MainEventBus : public EventBus {
	public:
		static MainEventBus* getInstance();
	protected:
		MainEventBus() {};
		static MainEventBus*	m_Instance;
	};
}

#define BIND_EVENT(instance, method) MainEventBus::getInstance()->subscribe(instance, &method)

#define EMITS_EVENTS enum class Events

#define EVENT : public Event<Events>

#define EVENT_TYPES Events

#define SET_EVENT_TYPE(x) \
public:\
virtual Events getType() const override {return Events::x;};

#define GET_EVENT_TYPE(x) x->getType()

#define POST_EVENT(event) MainEventBus::getInstance()->postEvent(static_cast<const Event<Events>*>(event)); 

#define SEND_EVENT(event) MainEventBus::getInstance()->sendEvent(static_cast<const Event<Events>*>(event));

#define FLUSH_EVENTS() MainEventBus::getInstance()->flush();
					

