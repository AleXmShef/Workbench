#pragma once

#ifdef WB_PLATFORM_WINDOWS
	#define NOMINMAX
	#include <Windows.h>
	#include <windowsx.h>
#endif

#include <iostream>
#include <iomanip>
#include <memory>
#include <algorithm>
#include <utility>
#include <functional>
#include <atomic>
#include <mutex>
#include <random>
#include <chrono>
#include <math.h>

#include <string>
#include <regex>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <queue>
#include <list>

#include <typeinfo>
#include <typeindex>
#include <type_traits>

#include <mathfu/vector.h>
#include <mathfu/matrix.h>
#include <mathfu/quaternion.h>
#include <mathfu/glsl_mappings.h>
//#include <mathfu/hlsl_mappings.h>

#undef UpdateResource

