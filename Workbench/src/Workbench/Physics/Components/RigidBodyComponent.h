#pragma once
#include "wbpch.h"
#include "ECS/ECSComponent.h"
#include "Physics/Primitives/Collider.h"
#include "Physics/Primitives/ForceAccumulator.h"

namespace Workbench {
	struct RigidBodyComponent : ECSComponent {
		RigidBodyComponent(
			const UUID* entity,
			float mass,
			mathfu::vec3 vel = { 0.0f, 0.0f, 0.0f },
			mathfu::vec3 ang_vel = { 0.0f, 0.0f, 0.0f },
			Collider3D* _collider = new BoxCollider({ 1.0f, 1.0f, 1.0f }, mathfu::mat4::Identity())
		) :
			ECSComponent(entity),
			inverse_mass(1.0f/mass),
			linear_velocity(vel),
			angular_velocity(ang_vel),
			collider(_collider)
		{
			float a = mass * 0.33f;
			mathfu::mat3 inertia_tensor(
				a, 0.0f, 0.0f,
				0.0f, a, 0.0f,
				0.0f, 0.0f, a
			);

			inverse_inertia_tensor_abc = mathfu::mat3(6.66f, 0.0f, 0.0f, 0.0f, 6.66f, 0.0f, 0.0f, 0.0f, 6.66f);
		};

		bool physicsEnabled = true;

		float inverse_mass = 1.0f;

		mathfu::vec3 linear_velocity;			//world space
		mathfu::vec3 angular_velocity;			//world space

		mathfu::vec3 linear_acceleration = {0.0f, 0.0f, 0.0f};		//world space
		mathfu::vec3 angular_acceleration = {0.0f, 0.0f, 0.0f};		//world space

		mathfu::mat3 inverse_inertia_tensor;	//world space
		mathfu::mat3 inverse_inertia_tensor_abc;

		Collider3D* collider;					//only box collider for now

		ForceAccumulator force_accumulator;

	};
}

