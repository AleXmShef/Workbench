#pragma once
#include "wbpch.h"
#include "Core.h"

#include "Physics/Primitives/Collider.h"
#include "Physics/Primitives/Contact.h"
#include "Physics/Components/RigidBodyComponent.h"

namespace Workbench {
	class CollisionDetector {
	public:
		static std::vector<Contact*> CheckForCollisions(RigidBodyComponent* body1, RigidBodyComponent* body2);

		static std::vector<Contact*> _contactBoxAndBox(BoxCollider* box1, BoxCollider* box2);
		static Contact* _contactBoxAndPoint(mathfu::vec3 point, BoxCollider* box);
	};
}

