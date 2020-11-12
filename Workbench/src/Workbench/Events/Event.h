#pragma once
#pragma warning(disable : 4251)
#include "wbpch.h"
#include "Core.h"

namespace Workbench {
	class WORKBENCH_API AbstractEvent {
	};

	template<class EventType>
	class WORKBENCH_API Event : public AbstractEvent {
	public:
		virtual EventType getType() const = 0;
	};

	

	

	
}

#define BIND_EVENT(instance, method) MainEventBus::getInstance()->subscribe(instance, &method)

#define BIND_EVENT_TO_LAMBDA(event_type, lamda) MainEventBus::getInstance()->subscribe<event_type>([](const Event<ECS::Events>* event) -> void lamda)

#define UNBIND_EVENT(instance, method) MainEventBus::getInstance()->unsubscribe(instance, &method)

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
					

