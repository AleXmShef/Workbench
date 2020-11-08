#pragma once
#include "wbpch.h"
#include "ECS/ECSComponent.h"

namespace Workbench {
	struct PhysicsComponent : ECSComponent {
		struct Data {
			float x = 0;
			float y = 0;
			float z = 0;

			float vel = 0;
			float acc = 0;
		};
		Data data;
	};
}

