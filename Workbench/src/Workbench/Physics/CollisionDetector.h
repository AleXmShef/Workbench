#pragma once
#include "wbpch.h"
#include "Core.h"

#include "Physics/Primitives/Collider.h"
#include "Physics/Primitives/Contact.h"
#include "Physics/Components/RigidBodyComponent.h"

namespace Workbench {
	class CollisionDetector {
	public:
		static std::vector<Contact> DetectContacts(const std::vector<RigidBodyComponent*>& bodies);

		static std::vector<Contact> CheckForCollisions(RigidBodyComponent* body1, RigidBodyComponent* body2);

		static std::vector<Contact> _contactBoxAndBox(BoxCollider* box1, BoxCollider* box2);

		static std::vector<Contact> _contactBoxAndHalfSpace(BoxCollider* box, HalfSpaceCollider* plane);
	};
}

