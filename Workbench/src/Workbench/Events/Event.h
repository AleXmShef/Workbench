#pragma once
#include "Core.h"

namespace Workbench {
	class Event {
	public:
		template<typename T>
		Event(T event_type) : m_EventType(event_type) {

		}

	protected:
	};

	//EMITS_EVENTS {
	//	None = 0,

	//	//Window events
	//	WindowClosed, WindowResized, WindowGainedFocus, WindowLostFocus, WindowMoved,

	//	//Input events
	//	KeyPressed, KeyReleased,
	//	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,

	//	//Engine events
	//	EngineTick, EngineUpdate, EngineRender
	//};

}

#define EMITS_EVENTS \
	using EventCallbackFn = std::function<void(Workbench::Event&)>;\
	protected: std::array<EventCallbackFn*> m_EventCallbacks;\
	public: virtual void AddEventCallback(const EventCallbackFn* callback) {m_EventCallbacks.push_back(callback)}\
			virtual void RemoveEventCallback(const EventCallbackFn* callback) {for(int i = 0; i < m_EventCallbacks.size(); i++) if(callback == _callback) m_EventCallback.erase(m_EventCallbacks.begin() + i);}\
	enum class Events