#include "wbpch.h"
#include "PhysicsSystem.h"
#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "ECS/ECS.h"

namespace Workbench {

	void PhysicsSystem::OnAttach() {
		BIND_EVENT(this, PhysicsSystem::OnPhysicsComponentChanged);
	}

	void PhysicsSystem::OnUpdate(WB_GAME_TIMER* timer) {
		auto [it, end] = ECS::getInstance()->GetComponents<PhysicsComponent>();
		for (; it != end; ++it) {
			auto physics = *it;
			auto transform = ECS::getInstance()->GetEntityComponent<TransformComponent>(physics->getEntityId());

			if (transform == nullptr) {
				WB_CORE_WARN("PhysicsComponent requires TransformComponent!");
				ECS::getInstance()->RemoveComponent(physics->getEntityId(), physics);
				return;
			}
			else {
				transform->position += physics->velocity;
				physics->velocity += physics->acceleration + m_GravityAcc;
			}
		}
	}

	void PhysicsSystem::OnPhysicsComponentChanged(const Event<ECS::Events>* event) {
		if (event->getType() == ECS::Events::EntityComponentsChangedEvent) {
			auto _event = static_cast<const ECS::EntityComponentsChangedEvent<PhysicsComponent>*>(event);
			if (_event) {
				switch (_event->getActionType()) {
				case ECS::EntityComponentsChangedEvent<PhysicsComponent>::ActionType::ComponentCreated:
					WB_CORE_LOG("Physics component created!");
					break;
				case ECS::EntityComponentsChangedEvent<PhysicsComponent>::ActionType::ComponentChanged:
					WB_CORE_LOG("Physics component changed!");
					break;
				case ECS::EntityComponentsChangedEvent<PhysicsComponent>::ActionType::ComponentDestroyed:
					WB_CORE_LOG("Physics component destroyed!");
					break;
				}
			}
		}
	}
}
