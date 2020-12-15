#pragma once
#include "wbpch.h"
#include "Core.h"
#include "Physics/Components/RigidBodyComponent.h"

namespace Workbench {
	struct Contact {
		RigidBodyComponent* bodies[2];
		mathfu::vec3 relative_contact_position[2];

		mathfu::vec3 contact_point;		//world space
		mathfu::vec3 contact_normal;	//world space
		mathfu::vec3 contact_velocity = {0.0f, 0.0f, 0.0f};
		float penetration_depth;

		float desiredDeltaVelocity = 0;

		bool operator<(const Contact&other) {
			return penetration_depth < other.penetration_depth;
		}
		mathfu::mat3 transform;
	};
}

