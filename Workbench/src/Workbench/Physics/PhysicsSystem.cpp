#include "wbpch.h"
#include "PhysicsSystem.h"
#include "Components/RigidBodyComponent.h"
#include "Components/TransformComponent.h"
#include "Renderer/Components/CameraComponent.h"
#include "Renderer/Components/MeshComponent.h"
#include "Renderer/MeshGenerator.h"
#include "CollisionDetector.h"
#include "ECS/ECS.h"

namespace Workbench {

	PhysicsSystem::PhysicsSystem(RenderSystem* render) {
		renderer = render;
	};

	void PhysicsSystem::OnAttach() {
		BIND_EVENT(this, PhysicsSystem::OnPhysicsComponentChanged);
	}

	mathfu::mat3 basisFromAxis(mathfu::vec3 x) {

		mathfu::vec3 contactTangent[2];

		if (fabs(x.x) > fabs(x.y))
		{
			// Scaling factor to ensure the results are normalised
			const float s = 1.0f / sqrt(x.z * x.z +
				x.x * x.x);

			// The new X-axis is at right angles to the world Y-axis
			contactTangent[0].x = x.z * s;
			contactTangent[0].y = 0;
			contactTangent[0].z = -x.x * s;

			// The new Y-axis is at right angles to the new X- and Z- axes
			contactTangent[1].x = x.y * contactTangent[0].x;
			contactTangent[1].y = x.z * contactTangent[0].x -
				x.x * contactTangent[0].z;
			contactTangent[1].z = -x.y * contactTangent[0].x;
		}
		else
		{
			// Scaling factor to ensure the results are normalised
			const float s = 1.0f / sqrt(x.z * x.z +
				x.y * x.y);

			// The new X-axis is at right angles to the world X-axis
			contactTangent[0].x = 0;
			contactTangent[0].y = -x.z * s;
			contactTangent[0].z = x.y * s;

			// The new Y-axis is at right angles to the new X- and Z- axes
			contactTangent[1].x = x.y * contactTangent[0].z -
				x.z * contactTangent[0].y;
			contactTangent[1].y = -x.x * contactTangent[0].z;
			contactTangent[1].z = x.x * contactTangent[0].y;
		}

		mathfu::mat3 mat = mathfu::mat3(
			x.x, x.y, x.z,
			contactTangent[0].x, contactTangent[0].y, contactTangent[0].z,
			contactTangent[1].x, contactTangent[1].y, contactTangent[1].z);

		return mat;
	}

	void calculateInternals(Contact& contact, float tickTime) {
		contact.transform = basisFromAxis(contact.contact_normal);

		for (int i = 0; i < 2; i++) if (contact.bodies[i]) {
			auto body = contact.bodies[i];
			auto body_transform = ECS::GetInstance()->GetEntityComponent<TransformComponent>(body->getEntityId());

			mathfu::vec3 relativeContactPosition = contact.contact_point - body_transform->position.xyz();

			mathfu::vec3 velocity = mathfu::vec3::CrossProduct(body->angular_velocity, relativeContactPosition);

			velocity += body->linear_velocity;

			velocity = velocity * contact.transform;

			contact.contact_velocity += (i == 0 ? 1 : -1) * velocity;
		}
	}

	void calculateDesiredVelocity(Contact& contact, float tickTime) {
		float velocityFromAcc = 0;

		for (int i = 0; i < 2; i++) if (contact.bodies[i]) {
			auto body = contact.bodies[i];
			auto body_transform = ECS::GetInstance()->GetEntityComponent<TransformComponent>(body->getEntityId());

			mathfu::vec3 relativeContactPosition = contact.contact_point - body_transform->position.xyz();

			velocityFromAcc += (i == 0 ? 1 : -1) * mathfu::vec3::DotProduct(body->linear_acceleration * tickTime, contact.contact_normal);
		}

		float restitution = 0.6f;

		contact.desiredDeltaVelocity = -contact.contact_velocity.x - restitution * (contact.contact_velocity.x);
	}

	struct VelResolve {
		mathfu::vec3 velchange[2];
		mathfu::vec3 rotchange[2];
	};

	struct PosResolve {
		mathfu::vec3 linchange[2];
		mathfu::vec3 angchange[2];
	};

	PosResolve resolvePosition(Contact& contact) {

		float angular_move[2];
		float linear_move[2];

		float totalInertia = 0.0f;;
		float angular_inertia[2];
		float linear_inertia[2];

		for (int i = 0; i < 2; i++) if (contact.bodies[i]) {
			auto body = contact.bodies[i];
			auto body_transform = ECS::GetInstance()->GetEntityComponent<TransformComponent>(body->getEntityId());

			mathfu::vec3 relativeContactPosition = contact.contact_point - body_transform->position.xyz();

			mathfu::vec3 angInertiaWorld = mathfu::vec3::CrossProduct(relativeContactPosition, contact.contact_normal);

			angInertiaWorld = angInertiaWorld * body->inverse_inertia_tensor;

			angInertiaWorld = mathfu::vec3::CrossProduct(angInertiaWorld, relativeContactPosition);

			angular_inertia[i] = mathfu::vec3::DotProduct(angInertiaWorld, contact.contact_normal);

			linear_inertia[i] = body->inverse_mass;

			totalInertia += angular_inertia[i] + linear_inertia[i];
		}

		PosResolve position_resolve;

		for (int i = 0; i < 2; i++) if (contact.bodies[i]) {
			auto body = contact.bodies[i];
			auto body_transform = ECS::GetInstance()->GetEntityComponent<TransformComponent>(body->getEntityId());

			mathfu::vec3 relativeContactPosition = contact.contact_point - body_transform->position.xyz();

			float sign = (i == 0 ? 1 : -1);

			angular_move[i] = sign * contact.penetration_depth * (angular_inertia[i] / totalInertia);
			linear_move[i] = sign * contact.penetration_depth * (linear_inertia[i] / totalInertia);

			mathfu::vec3 proj = relativeContactPosition;

			proj += contact.contact_normal * mathfu::vec3::DotProduct(-relativeContactPosition, contact.contact_normal);

			float angularLimit = 0.2f;

			float maxMag = angularLimit * proj.Length();

			if (angular_move[i] < -maxMag) {
				float totalMove = angular_move[i] + linear_move[i];
				angular_move[i] = -maxMag;
				linear_move[i] = totalMove - angular_move[i];
			}
			else if (angular_move[i] > maxMag) {
				float totalMove = angular_move[i] + linear_move[i];
				angular_move[i] = maxMag;
				linear_move[i] = totalMove - angular_move[i];
			}

			mathfu::vec3 targetAngDirection = mathfu::vec3::CrossProduct(relativeContactPosition, contact.contact_normal);

			position_resolve.angchange[i] = targetAngDirection * body->inverse_inertia_tensor * (angular_move[i] / angular_inertia[i]);

			position_resolve.linchange[i] = contact.contact_normal * linear_move[i];

			body_transform->position += mathfu::vec4(position_resolve.linchange[i], 0.0f);

			RotateQuat(body_transform->rotation, position_resolve.angchange[i], 1.0f);

			body_transform->rebuildWorldMatrix();

			body->collider->transform = body_transform->worldMatrix;

			body->inverse_inertia_tensor = body_transform->rotation.ToMatrix().Inverse().Transpose() * body->inverse_inertia_tensor_abc * body_transform->rotation.ToMatrix().Inverse();
		}

		return position_resolve;
	}

	VelResolve resolveVelocity(Contact& contact) {

		mathfu::vec3 impulse;

		float deltaVelocity = 0;

		for (int i = 0; i < 2; i++) if (contact.bodies[i]) {
			auto body = contact.bodies[i];
			auto body_transform = ECS::GetInstance()->GetEntityComponent<TransformComponent>(body->getEntityId());

			mathfu::vec3 relativeContactPosition = contact.contact_point - body_transform->position.xyz();

			mathfu::vec3 deltaVelWorld = mathfu::vec3::CrossProduct(relativeContactPosition, contact.contact_normal);

			deltaVelWorld = deltaVelWorld * body->inverse_inertia_tensor;
			deltaVelWorld = mathfu::vec3::CrossProduct(deltaVelWorld, relativeContactPosition);

			deltaVelocity += mathfu::vec3::DotProduct(deltaVelWorld, contact.contact_normal);

			deltaVelocity += body->inverse_mass;
		}

		impulse = { contact.desiredDeltaVelocity / deltaVelocity, 0.0f, 0.0f };

		impulse = impulse * contact.transform.Inverse();

		VelResolve _thing;

		for (int i = 0; i < 2; i++) if (contact.bodies[i]) {
			auto body = contact.bodies[i];
			auto body_transform = ECS::GetInstance()->GetEntityComponent<TransformComponent>(body->getEntityId());

			mathfu::vec3 relativeContactPosition = contact.contact_point - body_transform->position.xyz();

			mathfu::vec3 impulsiveTorque = mathfu::vec3::CrossProduct(relativeContactPosition, impulse);

			mathfu::vec3 rotationChange = (body->inverse_inertia_tensor * impulsiveTorque) * (i == 0 ? 1 : -1);

			mathfu::vec3 velocityChange = impulse * (i == 0 ? 1 : -1);

			body->linear_velocity += velocityChange;
			body->angular_velocity += rotationChange;
			_thing.velchange[i] = velocityChange;
			_thing.rotchange[i] = rotationChange;
		}
		return _thing;
	}

	void PhysicsSystem::ResolveContacts(std::vector<Contact>& contacts, float tickTime) {
		for (auto& contact : contacts) {
			calculateInternals(contact, tickTime);
			calculateDesiredVelocity(contact, tickTime);
		}

		ResolvePositions(contacts, tickTime);

		ResolveVelocities(contacts, tickTime);
	}

	void PhysicsSystem::ResolvePositions(std::vector<Contact>& contacts, float tickTime) {
		int iterations = 2048;

		int cur_it = 0;

		while (cur_it < iterations) {

			contacts;

			float max = 0.001f;
			int index = -1;

			for (int i = 0; i < contacts.size(); i++) {
				if (contacts[i].penetration_depth > max) {
					max = contacts[i].desiredDeltaVelocity;
					index = i;
				}
			}

			if (index >= 0) {
				auto posresolve = resolvePosition(contacts[index]);

				for (int i = 0; i < contacts.size(); i++) {
					for (int b = 0; b < 2; b++) if (contacts[i].bodies[b] != nullptr) {
						for (int d = 0; d < 2; d++) {
							if (contacts[i].bodies[b]->getUuid() == contacts[index].bodies[d]->getUuid()) {
								if (contacts[i].bodies[b]->physicsEnabled) {
									auto body = contacts[i].bodies[b];
									auto body_transform = ECS::GetInstance()->GetEntityComponent<TransformComponent>(body->getEntityId());

									mathfu::vec3 relativeContactPosition = contacts[i].contact_point - body_transform->position.xyz();

									mathfu::vec3 deltaPos = posresolve.linchange[d] + mathfu::vec3::CrossProduct(posresolve.angchange[d], relativeContactPosition);

									// The sign of the change is positive if we're
									// dealing with the second body in a contact
									// and negative otherwise (because we're
									// subtracting the resolution)..
									contacts[i].penetration_depth += mathfu::vec3::DotProduct(deltaPos, contacts[i].contact_normal) * (b ? 1 : -1);
								}
							}
						}
					}
				}
			}
			else {
				break;
			}
			cur_it++;
		}
	}

	void PhysicsSystem::ResolveVelocities(std::vector<Contact>& contacts, float tickTime) {

		int iterations = 2048;

		int cur_it = 0;

		while (cur_it < iterations) {

			contacts;

			float max = 0.01f;
			int index = -1;

			for (int i = 0; i < contacts.size(); i++) {
				if (fabs(contacts[i].desiredDeltaVelocity) > max) {
					max = fabs(contacts[i].desiredDeltaVelocity);
					index = i;
				}
			}

			if (index >= 0) {
				auto _thing = resolveVelocity(contacts[index]);

				for (int i = 0; i < contacts.size(); i++) {
					for (int b = 0; b < 2; b++) if (contacts[i].bodies[b] != nullptr) {
						for (int d = 0; d < 2; d++) {
							if (contacts[i].bodies[b]->getUuid() == contacts[index].bodies[d]->getUuid()) {
								if (contacts[i].bodies[b]->physicsEnabled) {
									auto body = contacts[i].bodies[b];
									auto body_transform = ECS::GetInstance()->GetEntityComponent<TransformComponent>(body->getEntityId());

									mathfu::vec3 relativeContactPosition = contacts[i].contact_point - body_transform->position.xyz();

									mathfu::vec3 deltaVel = _thing.velchange[d] + mathfu::vec3::CrossProduct(_thing.rotchange[d], relativeContactPosition);

									deltaVel = (deltaVel * contacts[i].transform) * (b ? -1 : 1);

									contacts[i].contact_velocity += deltaVel;

									calculateDesiredVelocity(contacts[i], tickTime);
								}
							}
						}
					}
				}
			}
			else {
				break;
			}
			cur_it++;
		}
	}

	void PhysicsSystem::OnUpdate(WB_GAME_TIMER* timer) {
		auto rigidBodies = ECS::GetInstance()->GetComponents<RigidBodyComponent>();
		for (auto body : rigidBodies) {
			auto transform = ECS::GetInstance()->GetEntityComponent<TransformComponent>(body->getEntityId());
			if (body->physicsEnabled && transform) {
				Integrate(timer->GetTickTime(), body, transform);
			}
			auto mesh2 = ECS::GetInstance()->GetEntityComponent<MeshComponent>(body->getEntityId());
			mesh2->GetMesh()->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		}
		
		if (rigidBodies.size() > 1) {
			auto contacts = CollisionDetector::DetectContacts(rigidBodies);
			
			if (!contacts.empty()) {
				for (auto& contact : contacts) {
					if (contact.bodies[0]) {
						auto mesh1 = ECS::GetInstance()->GetEntityComponent<MeshComponent>(contact.bodies[0]->getEntityId());
						mesh1->GetMesh()->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
					}
					if (contact.bodies[1]) {
						auto mesh2 = ECS::GetInstance()->GetEntityComponent<MeshComponent>(contact.bodies[1]->getEntityId());
						mesh2->GetMesh()->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
					}
					
				}
			ResolveContacts(contacts, timer->GetTickTime());
			}
			updateContactCubes(contacts);
		}
		
	}

	void PhysicsSystem::updateContactCubes(std::vector<Contact>& contacts) {
		if (contacts.size() < contactCubes.size()) {
			for (int i = contactCubes.size(); i > contacts.size(); i--) {
				ECS::GetInstance()->DestroyEntity(contactCubes[i - 1]->getEntityId());
				contactCubes.erase(contactCubes.begin() + (i - 1));
			}
		}
		else if (contacts.size() > contactCubes.size()) {
			for (int i = 0; i < contacts.size() - contactCubes.size(); i++) {
				auto newCube = ECS::GetInstance()->CreateEntity();
				auto newTransform = ECS::GetInstance()->CreateComponentForEntity<TransformComponent>(newCube);
				ECS::GetInstance()->CreateComponentForEntity<MeshComponent>(newCube, MeshGenerator::CreateBox(0.2, {0.0f, 1.0f, 0.0f, 0.0f}));
				contactCubes.push_back(newTransform);
			}
		}
		if (contacts.size() > 0) {
			for (int i = 0; i < contactCubes.size(); i++) {
				contactCubes[i]->position = { contacts[i].contact_point, 0.0f };
				contactCubes[i]->rebuildWorldMatrix();
			}
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
		//body->linear_acceleration = body->linear_acceleration;
		body->linear_velocity += body->linear_acceleration * tickTime;

		//update angular velocity and acceleration
		//body->angular_acceleration = { 0.0f, 0.0f, 0.0f };
		body->angular_velocity += body->angular_acceleration * tickTime;

		//update position
		transform->position += mathfu::vec4(body->linear_velocity * tickTime, 0.0f);

		//update orientation
		RotateQuat(transform->rotation, body->angular_velocity, tickTime);

		transform->rebuildWorldMatrix();

		body->collider->transform = transform->worldMatrix;

		body->inverse_inertia_tensor = transform->rotation.ToMatrix().Inverse().Transpose() * body->inverse_inertia_tensor_abc * transform->rotation.ToMatrix().Inverse();
	}
}
