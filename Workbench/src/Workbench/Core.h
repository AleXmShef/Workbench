#pragma once
#include "wbpch.h"

#if defined(DEBUG) || defined(_DEBUG)
	#define WB_DEBUG
#endif

#ifndef LOG_LEVEL
	#define LOG_LEVEL Logger::log_level::regular
#endif
#define CORE_LOG_LEVEL Logger::log_level::trace
#define RENDERER_LOG_LEVEL Logger::log_level::regular

#ifdef WB_PLATFORM_WINDOWS
	#ifdef WB_BUILD_DLL 
		#define WORKBENCH_API __declspec(dllexport)
	#else
		#define WORKBENCH_API __declspec(dllimport)
	#endif
#else
	
#endif
