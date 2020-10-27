#pragma once
#include "Core.h"
#include "Events/Event.h"

namespace Workbench {
	class WORKBENCH_API Window {
	public:

		EMITS_EVENTS {
			WindowCreatedEvent = 0,
			WindowBeganResizeEvent,
			WindowResizedEvent,
			WindowLostFocusEvent,
			WindowGainedFocusEvent,
			WindowDestroyedEvent,

			WindowMouseMovedEvent
		};

		class WindowCreatedEvent EVENT {
		SET_EVENT_TYPE(WindowCreatedEvent)
		public:
			explicit WindowCreatedEvent(Window* window) : window_instance(window) {};
			const Window* getWindow() const { return window_instance; }
		protected:
			Window* window_instance = nullptr;
		};

		class WindowDestroyedEvent EVENT {
		SET_EVENT_TYPE(WindowDestroyedEvent)
		public:
			explicit WindowDestroyedEvent(Window* window) : window_instance(window) {};
			const Window* getWindow() const { return window_instance; }
		protected:
			Window* window_instance = nullptr;
		};

		class WindowBeganResizeEvent EVENT {
		SET_EVENT_TYPE(WindowBeganResizeEvent)
		};

		class WindowResizedEvent EVENT {
		SET_EVENT_TYPE(WindowResizedEvent)
		public:
			WindowResizedEvent(uint32_t width, uint32_t height) : dimensions({width, height}) {};
			std::pair<uint32_t, uint32_t> dimensions;
		};

		class WindowMouseMovedEvent EVENT {
		SET_EVENT_TYPE(WindowMouseMovedEvent)
		public:
			WindowMouseMovedEvent(uint32_t _xPos, uint32_t _yPos) : xPos(_xPos), yPos(_yPos) {};
			std::pair<uint32_t, uint32_t> getCoords() { return { xPos, yPos }; };
		protected:
			uint32_t xPos, yPos;
		};

		struct WindowProps {
			std::string windowTitle = "Workbench Sandbox";
			uint32_t windowWidth = 1280;
			uint32_t windowHeight = 720;
			bool isFullScreen = false;
			bool isVsync = true;
		};
	public:
		virtual ~Window() {};

		virtual void OnUpdate() = 0;
		virtual void OnClose() = 0;

		virtual std::pair<uint32_t, uint32_t> GetDimensions() const = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() = 0;
	};
}
