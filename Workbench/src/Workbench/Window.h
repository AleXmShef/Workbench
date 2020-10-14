#pragma once
#include "Core.h"

class WORKBENCH_API Window {
public:
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

	virtual std::pair<uint32_t, uint32_t> GetDimensions() const = 0;

	virtual void SetVSync(bool enabled) = 0;
	virtual bool IsVSync() const = 0;

	virtual void* GetNativeWindow() = 0;
};
