#pragma once
#include <Workbench.h>

#include <Physics/Components/TransformComponent.h>
#include <Physics/Components/RigidBodyComponent.h>
#include <Renderer/Components/MeshComponent.h>
#include <Renderer/Components/CameraComponent.h>
#include <Renderer/MeshGenerator.h>
#include <Random/RandomGenerator.h>

#include "OrbitCamera.h"

using namespace Workbench;

class CollisionDetectionTest : public Workbench::Layer {
public:
	CollisionDetectionTest(Workbench::Engine* engine) : m_Engine(engine) {};

	void OnAttach() override {

		world = ECS::GetInstance();

		m_camera = new OrbitCamera(m_Engine);

		cube1 = m_Engine->GetWorld()->CreateEntity();
		cube2 = m_Engine->GetWorld()->CreateEntity();

		world->CreateComponentForEntity<TransformComponent>(cube1, mathfu::vec3(-2.0f, 1.1f, 0.0f));
		world->CreateComponentForEntity<TransformComponent>(cube2, mathfu::vec3(0.0f, 0.0f, 0.0f));

		world->CreateComponentForEntity<RigidBodyComponent>(cube1, 1.0f);
		world->CreateComponentForEntity<RigidBodyComponent>(cube2, 1.0f);

		world->CreateComponentForEntity<MeshComponent>(cube1, MeshGenerator::CreateBox());
		world->CreateComponentForEntity<MeshComponent>(cube2, MeshGenerator::CreateBox());
	}

	void OnUpdate(WB_GAME_TIMER* timer) override {
		if (cube1 != nullptr) {
			auto transform = world->GetEntityComponent<TransformComponent>(cube1);

			if (transform->position.x > 1.0f) {
				world->DestroyEntity(cube1);
				cube1 = m_Engine->GetWorld()->CreateEntity();
				world->CreateComponentForEntity<TransformComponent>(cube1, mathfu::vec3(-2.0f, 1.1f, 0.0f));
				world->CreateComponentForEntity<RigidBodyComponent>(cube1, 1.0f);
				world->CreateComponentForEntity<MeshComponent>(cube1, MeshGenerator::CreateBox());
			}
			else {
				transform->position.x += 0.2 * timer->GetTickTime();
				transform->rotation = transform->rotation * mathfu::quat::FromEulerAngles(0.0f, 0.0f, 0.5f * timer->GetTickTime());
				transform->rebuildWorldMatrix();
			}
		}
	}
protected:
	OrbitCamera* m_camera = nullptr;

	ECS* world = nullptr;

	const Workbench::UUID* cube1 = nullptr;
	const Workbench::UUID* cube2 = nullptr;

	Workbench::Engine* m_Engine = nullptr;
};
