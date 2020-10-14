#pragma once
#include "wbpch.h"

#ifdef WB_PLATFORM_WINDOWS
	#ifdef WB_BUILD_DLL 
		#define WORKBENCH_API __declspec(dllexport)
	#else
		#define WORKBENCH_API __declspec(dllimport)
	#endif

	#include "Platform/Windows/WindowsWindow.h"
	#define WB_CREATE_NATIVE_WINDOW(x) new WindowsWindow(x)
#else
	
#endif
