#pragma once
#include "wbpch.h"
#include "ECS/ECSEntity.h"
#include "ECS/ECSComponent.h"

namespace Workbench {
	class ECSSystem {
	public:
		ECSSystem() = default;
		virtual ~ECSSystem() = default;
	};
}

