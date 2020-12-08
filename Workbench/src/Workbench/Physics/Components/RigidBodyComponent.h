#pragma once
#include "wbpch.h"
#include "ECS/ECSComponent.h"

namespace Workbench {
	struct RigidBodyComponent : ECSComponent {
		RigidBodyComponent(
			const UUID* entity, 
			float mass, 
			mathfu::vec3 vel = {0.0f, 0.0f, 0.0f},
			mathfu::vec3 ang_vel = { 0.0f, 0.0f, 0.0f },
			mathfu::mat3 inertia_tensor = mathfu::mat3::Identity()
		) :
			ECSComponent(entity),
			inverse_mass(1.0f/mass),
			linear_velocity(vel),
			angular_velocity(ang_vel),
			inverse_inertia_tensor(inertia_tensor.Inverse())
		{
			
		};

		bool physicsEnabled = true;

		float inverse_mass;

		mathfu::vec3 linear_velocity;			//world space
		mathfu::vec3 angular_velocity;			//world space

		mathfu::vec3 linear_acceleration;		//world space
		mathfu::vec3 angular_acceleration;		//world space

		mathfu::mat3 inverse_inertia_tensor;	//local space

	};
}

