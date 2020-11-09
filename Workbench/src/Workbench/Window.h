#pragma once
#include "Core.h"
#include "Events/EventBus.h"
#include "Input/Keycodes.h"
#include "Layers/Layer.h"
#include "Logger/Logger.h"
#include "Time/Timer.h"

namespace Workbench {
	class WORKBENCH_API Window : public Layer {
	public:

		EMITS_EVENTS {
			WindowCreatedEvent = 0,
			WindowBeganResizeEvent,
			WindowResizedEvent,
			WindowLostFocusEvent,
			WindowGainedFocusEvent,
			WindowDestroyedEvent,

			WindowMouseMovedEvent,
			WindowMouseButtonPressedEvent,
			WindowMouseButtonReleasedEvent,

			WindowButtonPressedEvent,
			WindowButtonReleasedEvent
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

		class WindowLostFocusEvent EVENT {
		SET_EVENT_TYPE(WindowLostFocusEvent)
		};

		class WindowGainedFocusEvent EVENT {
		SET_EVENT_TYPE(WindowGainedFocusEvent)
		};

		class WindowMouseMovedEvent EVENT {
		SET_EVENT_TYPE(WindowMouseMovedEvent)
		public:
			WindowMouseMovedEvent(uint32_t _xPos, uint32_t _yPos) : xPos(_xPos), yPos(_yPos) {};
			std::pair<uint32_t, uint32_t> getCoords() { return { xPos, yPos }; };
		protected:
			uint32_t xPos, yPos;
		};

		class WindowMouseButtonPressedEvent EVENT {
		SET_EVENT_TYPE(WindowMouseButtonPressedEvent)
		public:
			WindowMouseButtonPressedEvent(WB_KEYCODES mouse_button) : m_mouse_button(mouse_button) {};
			WB_KEYCODES getButton() { return m_mouse_button; };
		protected:
			WB_KEYCODES m_mouse_button;
		};

		class WindowButtonPressedEvent EVENT {
			SET_EVENT_TYPE(WindowButtonPressedEvent)
		public:
			WindowButtonPressedEvent(WB_KEYCODES button) : m_button(button) {};
			inline WB_KEYCODES getButton() { return m_button; };
		protected:
			WB_KEYCODES m_button;
		};

		class WindowButtonReleasedEvent EVENT {
		SET_EVENT_TYPE(WindowButtonReleasedEvent)
		public:
			WindowButtonReleasedEvent(WB_KEYCODES button) : m_button(button) {};
			inline WB_KEYCODES getButton() { return m_button; };
		protected:
			WB_KEYCODES m_button;
		};

		struct WindowProps {
			std::string windowTitle = "Workbench Sandbox";
			uint32_t windowWidth = 1280;
			uint32_t windowHeight = 720;
			bool isFullScreen = false;
			bool isVsync = true;
		};
	public:
		virtual ~Window() {
			
		};

		virtual bool checkForButtonPress(WB_KEYCODES button) = 0;

		virtual void OnUpdate(WB_GAME_TIMER* timer) override = 0;
		virtual void OnClose() = 0;

		virtual std::pair<uint32_t, uint32_t> GetDimensions() const = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void ToggleFullscreen() = 0;
		virtual bool IsFullscreen() const = 0;

		virtual void* GetNativeWindow() = 0;
	};
}

#ifdef WB_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#define WB_CREATE_NATIVE_WINDOW(x) new WindowsWindow(x)
#endif
