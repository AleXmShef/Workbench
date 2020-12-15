#pragma once
#include "wbpch.h"
#include "Layers/Layer.h"
#include "ECS/ECS.h"
#include "Physics/Components/RigidBodyComponent.h"
#include "Physics/Components/TransformComponent.h"
#include "Physics/Primitives/Contact.h"
#include "Renderer/RenderSystem.h"

#define WB_PHYSICS_G -9.81f

namespace Workbench {

	class PhysicsSystem : public Layer {
	public:

		PhysicsSystem(RenderSystem* render);

		void OnAttach() override;
		void OnUpdate(WB_GAME_TIMER* timer) override;
	protected:
		void OnPhysicsComponentChanged(const Event<ECS::Events>* event);

		void ResolveContacts(std::vector<Contact>& contacts, float tickTime);

		void ResolvePositions(std::vector<Contact>& contacts, float tickTime);

		void ResolveVelocities(std::vector<Contact>& contacts, float tickTime);

		void Integrate(float tickTime, RigidBodyComponent* body, TransformComponent* transform);

	protected:
		mathfu::vec3 m_GravityAcc = mathfu::vec3(0, WB_PHYSICS_G, 0);

		RenderSystem* renderer = nullptr;
	};
}

