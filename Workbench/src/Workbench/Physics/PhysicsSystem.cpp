#include "wbpch.h"
#include "PhysicsSystem.h"
#include "Components/RigidBodyComponent.h"
#include "Components/TransformComponent.h"
#include "Renderer/Components/CameraComponent.h"
#include "ECS/ECS.h"

namespace Workbench {

	void PhysicsSystem::OnAttach() {
		BIND_EVENT(this, PhysicsSystem::OnPhysicsComponentChanged);
	}

	void PhysicsSystem::OnUpdate(WB_GAME_TIMER* timer) {
		auto rigidBodies = ECS::GetInstance()->GetComponents<RigidBodyComponent>();
		for (auto body : rigidBodies) {
			auto transform = ECS::GetInstance()->GetEntityComponent<TransformComponent>(body->getEntityId());
			if (body->physicsEnabled && transform)
				Integrate(timer->GetTickTime(), body, transform);
		}
	}

	void PhysicsSystem::OnPhysicsComponentChanged(const Event<ECS::Events>* event) {
		if (event->getType() == ECS::Events::EntityComponentsChangedEvent) {
			auto _event = dynamic_cast<const ECS::EntityComponentsChangedEvent*>(event);
			if (_event && _event->GetComponentType() == typeid(RigidBodyComponent)) {
				switch (_event->GetActionType()) {
				case ECS::EntityComponentsChangedEvent::ActionType::ComponentCreated:
					WB_CORE_LOG("Physics component created!");
					break;
				case ECS::EntityComponentsChangedEvent::ActionType::ComponentChanged:
					WB_CORE_LOG("Physics component changed!");
					break;
				case ECS::EntityComponentsChangedEvent::ActionType::ComponentDestroyed:
					WB_CORE_LOG("Physics component destroyed!");
					break;
				}
			}
		}
	}

	void PhysicsSystem::Integrate(float tickTime, RigidBodyComponent* body, TransformComponent* transform) {

		//update linear velocity and acceleration
		body->linear_acceleration = { 0.0f, 0.0f, 0.0f };
		body->linear_velocity += m_GravityAcc * tickTime;

		//update angular velocity and acceleration
		body->angular_acceleration = { 0.0f, 0.0f, 0.0f };
		body->angular_velocity += body->angular_acceleration * tickTime;

		//update position
		transform->position += mathfu::vec4(body->linear_velocity * tickTime, 0.0f);

		//update orientation
		auto rotQuat = mathfu::quat(0, transform->rotation * body->angular_velocity * tickTime * 0.5f);
		transform->rotation += (transform->rotation * rotQuat);
		transform->rotation.Normalize();

		transform->rebuildWorldMatrix();
	}
}
