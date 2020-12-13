#pragma once
#include "wbpch.h"
#include "Core.h"
#include "Physics/Components/RigidBodyComponent.h"

namespace Workbench {
	struct Contact {
		RigidBodyComponent* body1;
		RigidBodyComponent* body2;

		mathfu::vec3 contact_point;		//world space
		mathfu::vec3 contact_normal;	//world space
		float penetration_depth;

		bool operator<(const Contact&other) {
			return penetration_depth < other.penetration_depth;
		}

		float separating_velocity;
		mathfu::mat4 transform;
	};
}

