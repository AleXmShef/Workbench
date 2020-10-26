#pragma once
#include "wbpch.h"
#include "Logger.h"

namespace Workbench {
	class Event {
	public:
		Event();
		~Event();
	};

	class HandlerFunctionBase {
	public:
		virtual ~HandlerFunctionBase();
		void exec(const Event* event);
		virtual bool operator==(const HandlerFunctionBase& right) = 0;
	protected:
		virtual void call(const Event* event) = 0;
	};

	template<class EventConsumer, class EventType>
	class MemberFunctionHandler : public HandlerFunctionBase {
	public:
		typedef void (EventConsumer::*MemberFn)(const EventType*);

		MemberFunctionHandler(EventConsumer* consumer_instance, MemberFn member_function) 
			: m_Consumer(consumer_instance), m_MemberFunction(member_function) {};

		virtual bool operator==(const HandlerFunctionBase& another) {
			auto _another = static_cast<const MemberFunctionHandler<EventConsumer, EventType>*>(&another);
			if (_another != nullptr) {
				if (m_Consumer == _another->m_Consumer && m_MemberFunction == _another->m_MemberFunction)
					return true;
			}
			return false;
		};
	protected:
		virtual void call(const Event* event) override {
			(m_Consumer->*m_MemberFunction)(static_cast<const EventType*>(event));
		}

	protected:
		EventConsumer* m_Consumer;
		MemberFn m_MemberFunction;
	};

	class EventBus {
	public:
		using EventConsumerMap = std::unordered_map<std::type_index, std::list<HandlerFunctionBase*>>;
		using EventQueue = std::queue<std::pair<std::type_index, Event*>>;
		using PendingDeletionList = std::list<std::pair<std::type_index, HandlerFunctionBase*>>;

		template<class EventConsumer, class EventType>
		void subscribe(EventConsumer* consumer, void (EventConsumer::* memFn)(const EventType*)) {
			m_EventConsumerMap[typeid(EventType)].push_back(new MemberFunctionHandler<EventConsumer, EventType>(consumer, memFn));
		};

		template<class EventConsumer, class EventType>
		void unsubscribe(EventConsumer* consumer, void (EventConsumer::* memFn)(const EventType*)) {
			HandlerFunctionBase* comp = new MemberFunctionHandler<EventConsumer, EventType>(consumer, memFn);
			m_PendingDeletionList.push_back({ typeid(EventType), comp });
		}

		template <class EventType>
		void sendEvent(const EventType* event) {
			propagateEvent((Event*)event, typeid(EventType));
		}

		template <class EventType>
		void postEvent(const EventType* event) {
			m_EventQueue.push({ typeid(EventType), (Event*)event });
		}

		void flush();
	protected:
		void deleteAllPending() {
			for (auto [type, handler] : m_PendingDeletionList) {
				auto __handler = handler;
				m_EventConsumerMap[type].remove_if([__handler](auto _handler) {return *__handler == *_handler; });
			}
		};
		void propagateEvent(Event* event, std::type_index type);
	protected:

		PendingDeletionList m_PendingDeletionList;
		EventConsumerMap m_EventConsumerMap;
		EventQueue m_EventQueue;
	};

	class MainEventBus : public EventBus {
	public:
		static MainEventBus* getInstance();
	protected:
		MainEventBus() {};
		static MainEventBus* m_Instance;
	};
}

#define EMITS_EVENTS	enum class Events;\
						class Event : public Workbench::Event \
						{\
							public: virtual Events getType() const  = 0;\
						};\
						enum class Events

#define EVENT : public Event

#define EVENT_TYPES Events

#define SET_EVENT_TYPE(x)\
public:\
	virtual Events getType() const override {return Events::x;};

#define GET_EVENT_TYPE(x) x->getType()

#define POST_EVENT(event) MainEventBus::getInstance()->postEvent((Event*)(event)); 

#define SEND_EVENT(event) MainEventBus::getInstance()->sendEvent((Event*)(event));

#define FLUSH_EVENTS() MainEventBus::getInstance()->flush();
					

