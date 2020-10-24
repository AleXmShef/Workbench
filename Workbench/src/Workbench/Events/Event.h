#pragma once
#include "wbpch.h"

namespace Workbench {
	class Event {
	public:
		Event() {};
		~Event() {};
	};

	class HandlerFunctionBase {
	public:
		virtual ~HandlerFunctionBase() {};
		void exec(const Event* event) {
			call(event);
		}
	protected:
		virtual void call(const Event* event) = 0;
	};

	template<class EventConsumer, class EventType>
	class MemberFunctionHandler : public HandlerFunctionBase {
	public:
		typedef void (EventConsumer::*MemberFn)(const EventType*);

		MemberFunctionHandler(EventConsumer* consumer_instance, MemberFn member_function) 
			: m_Consumer(consumer_instance), m_MemberFunction(member_function) {};

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
		using EventConsumerMap = std::unordered_map<std::type_index, std::vector<HandlerFunctionBase*>>;

		template<class EventConsumer, class EventType>
		void subscribe(EventConsumer* consumer, void (EventConsumer::* memFn)(const EventType*)) {
			m_EventConsumerMap[typeid(EventType)].push_back(new MemberFunctionHandler<EventConsumer, EventType>(consumer, memFn));
		};

		template<class EventConsumer>
		void unsubscribe(EventConsumer* consumer, class EventType) {
			
		}

		template <class EventType>
		void sendEvent(const EventType* event) {
			for (auto consumer_fn : m_EventConsumerMap[typeid(*event)]) {
				consumer_fn->exec(event);
			}
		}
	protected:
		EventConsumerMap m_EventConsumerMap;
	};

	class MainEventBus : public EventBus {
	public:
		static MainEventBus* getInstance() { return m_Instance; };
	protected:
		MainEventBus() {};
		static MainEventBus* m_Instance;
	};
}

