#pragma once
#include "wbpch.h"

namespace Workbench {
	class DynamicResource {
	public:
		DynamicResource() = default;
		virtual ~DynamicResource() {};
		virtual void UpdateResource(const void* data, uint64_t size, uint64_t elementSize, uint64_t elementCount);
		virtual void ReleaseResource() = 0;
	};
}

