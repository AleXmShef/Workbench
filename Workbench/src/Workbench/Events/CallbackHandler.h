#pragma once
#include "wbpch.h"
#include "Event.h"

namespace Workbench {

	class WORKBENCH_API AbstractCallbackHandler {
	public:
		virtual ~AbstractCallbackHandler();

		void exec(const AbstractEvent* event);

		virtual bool operator==(const AbstractCallbackHandler& right) = 0;

	protected:
		virtual void call(const AbstractEvent* event) = 0;
	};

	template<class Subscriber, class EventType>
	class WORKBENCH_API MemberCallbackHandler : public AbstractCallbackHandler {
	public:
		typedef void (Subscriber::* Callback)(const Event<EventType>*);

		MemberCallbackHandler(Subscriber* consumer_instance, Callback member_function)
			: m_subscriber(consumer_instance), m_callback(member_function) {};

		virtual bool operator==(const AbstractCallbackHandler& another) override {
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
		Subscriber* m_subscriber;
		Callback		m_callback;
	};

	template<class EventType>
	class WORKBENCH_API LambdaCallbackHandler : public AbstractCallbackHandler {
	public:
		//typedef Callback = std::function<void (const Event<EventType>*) >;

		LambdaCallbackHandler(std::function<void(const Event<EventType>*)> callback) : m_callback(callback) {};

		virtual bool operator==(const AbstractCallbackHandler& another) override {
			return false;
		};
	protected:
		virtual void call(const AbstractEvent* event) override {
			m_callback(static_cast<const Event<EventType>*>(event));
		}
	protected:
		std::function<void(const Event<EventType>*)> m_callback;
	};
 
}

