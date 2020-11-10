#pragma once
#include "wbpch.h"
#include "ECS/ECSComponent.h"

namespace Workbench {
	struct PhysicsComponent : ECSComponent {
		PhysicsComponent(const UUID* entity, float mass, mathfu::vec4 vel, mathfu::vec4 acc) : 
			ECSComponent(entity), 
			velocity(vel), 
			acceleration(acc) 
		{
			invertMass = 1.0f / mass;
		};


		float invertMass;

		mathfu::vec4 velocity;
		mathfu::vec4 acceleration;
	};
}

