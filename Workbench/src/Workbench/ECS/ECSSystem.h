#pragma once
#include "wbpch.h"
#include "ECSEntity.h"
#include "ECSComponent.h"

namespace Workbench {
	class ECSSystem {
	public:
		ECSSystem() = default;
		virtual ~ECSSystem() = default;
	};
}

