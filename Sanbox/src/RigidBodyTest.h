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

class RigidBodyTest : public Workbench::Layer {
public:
	RigidBodyTest(Workbench::Engine* engine) : m_Engine(engine) {};

	void buildCubes() {
		cube1 = m_Engine->GetWorld()->CreateEntity();
		cube2 = m_Engine->GetWorld()->CreateEntity();

		world->CreateComponentForEntity<TransformComponent>(cube1, mathfu::vec3(0.5f, 3.0f, 0.0f));
		auto transform = world->CreateComponentForEntity<TransformComponent>(cube2, mathfu::vec3(0.0f, 0.0f, 0.0f));

		auto rigid = world->CreateComponentForEntity<RigidBodyComponent>(cube1, 1.0f);
		rigid->linear_acceleration = { 0.0f, -9.81f, 0.0f };
		auto rigid2 = world->CreateComponentForEntity<RigidBodyComponent>(cube2, 1.0f);
		rigid2->physicsEnabled = false;
		rigid2->collider->transform = transform->worldMatrix;

		world->CreateComponentForEntity<MeshComponent>(cube1, MeshGenerator::CreateBox());
		world->CreateComponentForEntity<MeshComponent>(cube2, MeshGenerator::CreateBox());
	}

	void OnAttach() override {

		world = ECS::GetInstance();

		m_camera = new OrbitCamera(m_Engine);

		buildCubes();
	}

	void OnUpdate(WB_GAME_TIMER* timer) override {
		if (cube1 != nullptr) {
			auto transform = world->GetEntityComponent<TransformComponent>(cube1);

			if (transform->position.y < -3.0f) {
				world->DestroyEntity(cube1);
				world->DestroyEntity(cube2);
				buildCubes();
				return;
			}
		}
		if (cube2 != nullptr) {
			auto transform = world->GetEntityComponent<TransformComponent>(cube2);

			if (transform->position.y < -3.0f) {
				world->DestroyEntity(cube1);
				world->DestroyEntity(cube2);
				buildCubes();
				return;
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

