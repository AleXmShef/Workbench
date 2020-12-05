#include "wbpch.h"
#include "PhysicsSystem.h"
#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "Renderer/Components/CameraComponent.h"
#include "ECS/ECS.h"

namespace Workbench {

	void PhysicsSystem::OnAttach() {
		BIND_EVENT(this, PhysicsSystem::OnPhysicsComponentChanged);
	}

	void PhysicsSystem::OnUpdate(WB_GAME_TIMER* timer) {
		auto [it, end] = ECS::getInstance()->GetComponents<TransformComponent>();
		for (; it != end; ++it) {
			auto temp = *it;
			auto transform = ECS::getInstance()->GetEntityComponent<TransformComponent>(temp->getEntityId());
			auto camera = ECS::getInstance()->GetEntityComponent<CameraComponent>(temp->getEntityId());
			
			if (camera == nullptr) {
				//transform->rotation = mathfu::vec4(
				//	(float)transform->rotation.x,
				//	(float)transform->rotation.y + 0.02,
				//	(float)transform->rotation.z,
				//	(float)transform->rotation.w
				//);
				//transform->rebuildWorldMatrix();
			}
		}
	}

	void PhysicsSystem::OnPhysicsComponentChanged(const Event<ECS::Events>* event) {
		if (event->getType() == ECS::Events::EntityComponentsChangedEvent) {
			auto _event = dynamic_cast<const ECS::EntityComponentsChangedEvent<PhysicsComponent>*>(event);
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
