#pragma once
#include "wbpch.h"


//Build dependant macros
#if defined(DEBUG) || defined(_DEBUG)
	#define WB_DEBUG
#endif


//Utility & helper macros
#define CHECK_BIT(var, pos) ((var) & (1<<(pos)))


//Logging macros
#ifndef LOG_LEVEL
	#define LOG_LEVEL Logger::log_level::regular
#endif
#define CORE_LOG_LEVEL Logger::log_level::regular
#define RENDERER_LOG_LEVEL Logger::log_level::regular


//Platform dependent macros
#ifdef WB_PLATFORM_WINDOWS
	#ifdef WB_BUILD_DLL 
		#define WORKBENCH_API __declspec(dllexport)
	#else
		#define WORKBENCH_API __declspec(dllimport)
	#endif
#else
	
#endif
