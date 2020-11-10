#include "wbpch.h"
#include "PhysicsSystem.h"
#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "ECS/ECS.h"

namespace Workbench {
	void PhysicsSystem::OnUpdate(WB_GAME_TIMER* timer) {
		auto [it, end] = ECS::getInstance()->GetComponents<PhysicsComponent>();
		for (; it != end; ++it) {
			auto physics = *it;
			auto transform = ECS::getInstance()->GetEntityComponent<TransformComponent>(physics->getEntityId());

			transform->position += physics->velocity;
			physics->velocity += physics->acceleration + m_GravityAcc;
		}
	}
}
