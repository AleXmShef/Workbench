#pragma once

#include <iostream>




#ifdef WB_BUILD_DLL 
	#define WORKBENCH_API __declspec(dllexport)
#else
	#define WORKBENCH_API __declspec(dllimport)
#endif
