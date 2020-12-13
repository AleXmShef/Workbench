#include "wbpch.h"
#include "PhysicsSystem.h"
#include "Components/RigidBodyComponent.h"
#include "Components/TransformComponent.h"
#include "Renderer/Components/CameraComponent.h"
#include "Renderer/Components/MeshComponent.h"
#include "CollisionDetector.h"
#include "ECS/ECS.h"

namespace Workbench {

	void PhysicsSystem::OnAttach() {
		BIND_EVENT(this, PhysicsSystem::OnPhysicsComponentChanged);
	}

	mathfu::mat4 basisFromAxis(mathfu::vec3 x, mathfu::vec3 y = { 0.0f, 1.0f, 0.0f }) {
		mathfu::vec3 z = mathfu::vec3::CrossProduct(x, y);

		if (z.LengthSquared() == 0.0f) {
			return basisFromAxis(x, { 0.0f, 0.0f, 1.0f });
		}

		y = mathfu::vec3::CrossProduct(z, x);

		y.Normalize();
		z.Normalize();

		mathfu::mat4 _mat = mathfu::mat4(mathfu::vec4(x, 0.0f), mathfu::vec4(y, 0.0f), mathfu::vec4(z, 0.0f), mathfu::vec4(0.0f, 0.0f, 0.0f, 0.0f));
		auto transpose_mat = _mat.Transpose();
		return _mat;
	}

	void PhysicsSystem::ResolveContacts(std::vector<Contact>& contacts) {
		for (auto& contact : contacts) {
			contact.contact_normal.Normalize();
			contact.transform = basisFromAxis(contact.contact_normal);

			float velocityPerUnitImpulseContact_normal = 0;
			mathfu::vec3 separatingVelocity(0.0f, 0.0f, 0.0f);

			RigidBodyComponent* bodies[2] = {contact.body1, contact.body2};

			for (int i = 0; i < 2; i++) if (bodies[i]) {
				auto body = bodies[i];
			
				auto body_transform = ECS::GetInstance()->GetEntityComponent<TransformComponent>(body->getEntityId());
			
				mathfu::vec3 relativeContactPosition = contact.contact_point - body_transform->position.xyz();
			
				mathfu::vec3 torquePerUnitImpulse = mathfu::vec3::CrossProduct(relativeContactPosition,  (i == 0 ? 1.0f : -1.0f) * contact.contact_normal);
			
				mathfu::vec3 rotationPerUnitImpulse = torquePerUnitImpulse * body->inverse_inertia_tensor;
			
				mathfu::vec3 velocityPerUnitImpulse = mathfu::vec3::CrossProduct(rotationPerUnitImpulse, relativeContactPosition);
			
				velocityPerUnitImpulseContact_normal += (velocityPerUnitImpulse * mathfu::mat3::ToRotationMatrix(contact.transform)).x;
			
				velocityPerUnitImpulseContact_normal += body->inverse_mass;
			
				separatingVelocity += mathfu::vec3::CrossProduct(body->angular_velocity, relativeContactPosition);
				separatingVelocity += body->linear_velocity;
			};
			
			float separatingVelocity_normal = (separatingVelocity * mathfu::mat3::ToRotationMatrix(contact.transform)).x;
			
			if (separatingVelocity_normal > 0)	//objects are moving apart
				return;
			
			float restitution = 0.2f;
			
			float deltaVelocity = -1*separatingVelocity_normal * (1 + restitution);
			
			mathfu::vec3 impulse = { deltaVelocity / velocityPerUnitImpulseContact_normal , 0.0f, 0.0f};
			
			impulse = impulse * mathfu::mat3::ToRotationMatrix(contact.transform).Inverse();
			
			for (int i = 0; i < 2; i++) if (bodies[i]) {
				
				impulse = impulse * ((i == 0) ? 1.0f : -1.0f);
				
				auto body = bodies[i];
				auto body_transform = ECS::GetInstance()->GetEntityComponent<TransformComponent>(body->getEntityId());
				
				mathfu::vec3 linearChange = impulse * body->inverse_mass;
				
				mathfu::vec3 impulsiveTorque = mathfu::vec3::CrossProduct(impulse, contact.contact_point - body_transform->position.xyz());
				mathfu::vec3 rotationalChange = impulsiveTorque * body->inverse_inertia_tensor;
				
				if (body->physicsEnabled) {
					body->linear_velocity += linearChange;
					body->angular_velocity += mathfu::vec3(rotationalChange.z, rotationalChange.y, -rotationalChange.x);
				}
			}

			float angInertia[2];
			float linInertia[2];
			
			float totalInertia = 0;
			
			for (int i = 0; i < 2; i++) if (bodies[i] && bodies[i]->physicsEnabled) {
				auto body = bodies[i];
				auto body_transform = ECS::GetInstance()->GetEntityComponent<TransformComponent>(body->getEntityId());
			
				mathfu::vec3 _angInertia = mathfu::vec3::CrossProduct(contact.contact_point - body_transform->position.xyz(), contact.contact_normal);
				_angInertia = _angInertia * body->inverse_inertia_tensor;
				_angInertia = mathfu::vec3::CrossProduct(_angInertia, contact.contact_point - body_transform->position.xyz());
			
				angInertia[i] = mathfu::vec3::DotProduct(_angInertia, contact.contact_normal);
			
				linInertia[i] = body->inverse_mass;
			
				totalInertia += linInertia[i] + angInertia[i];
			}
			
			float invInertia = 1.0f / totalInertia;
			
			float linearMove[2];
			float angularMove[2];
			
			linearMove[0] = contact.penetration_depth * linInertia[0] * invInertia;
			linearMove[1] = -1*contact.penetration_depth * linInertia[1] * invInertia;
			angularMove[0] = contact.penetration_depth * angInertia[0] * invInertia;
			angularMove[1] = -1*contact.penetration_depth * angInertia[1] * invInertia;
			
			for (int i = 0; i < 2; i++) if (bodies[i] && bodies[i]->physicsEnabled) {
				auto body = bodies[i];
				auto body_transform = ECS::GetInstance()->GetEntityComponent<TransformComponent>(body->getEntityId());
			
				body_transform->position += (contact.contact_normal * linearMove[i], 0.0f);
			
				mathfu::vec3 impTorque = mathfu::vec3::CrossProduct(contact.contact_point - body_transform->position.xyz(), contact.contact_normal);
				mathfu::vec3 impPerMove = impTorque * body->inverse_inertia_tensor;
			
				mathfu::vec3 rotPerMove = impPerMove * 1.0f / angInertia[i];
			
				mathfu::vec3 rot = rotPerMove * angularMove[i];
			
				auto rotQuat = mathfu::quat(0, rot * 0.5f);
				body_transform->rotation += (body_transform->rotation * rotQuat);
				body_transform->rotation.Normalize();
			
			
				body_transform->rebuildWorldMatrix();
			}
			
		}
	}

	void PhysicsSystem::OnUpdate(WB_GAME_TIMER* timer) {
		auto rigidBodies = ECS::GetInstance()->GetComponents<RigidBodyComponent>();
		for (auto body : rigidBodies) {
			auto transform = ECS::GetInstance()->GetEntityComponent<TransformComponent>(body->getEntityId());
			if (body->physicsEnabled && transform)
				Integrate(timer->GetTickTime(), body, transform);
		}

		if (rigidBodies.size() > 1) {
			auto contacts = CollisionDetector::DetectContacts(rigidBodies);

			if (!contacts.empty()) {
				WB_CORE_LOG("Contact!");
			}
		
			ResolveContacts(contacts);
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

		body->inverse_inertia_tensor = body->inverse_inertia_tensor_abc * transform->rotation.ToMatrix();

		//update linear velocity and acceleration
		//body->linear_acceleration = body->linear_acceleration;
		body->linear_velocity += body->linear_acceleration * tickTime;

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

		body->collider->transform = transform->worldMatrix;
	}
}
