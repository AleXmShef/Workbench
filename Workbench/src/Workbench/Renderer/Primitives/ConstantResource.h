#pragma once
#include "wbpch.h"

namespace Workbench {
	class ConstantResource {
	public:
		ConstantResource() = default;
		virtual ~ConstantResource() {};

		virtual void UpdateResource(const void* data, uint64_t size) {};

		virtual void UploadResource();

		virtual void ReleaseResource() = 0;
	};
}

