#pragma once
#include <Workbench.h>

#include <Physics/Components/TransformComponent.h>
#include <Physics/Components/RigidBodyComponent.h>
#include <Renderer/Components/MeshComponent.h>
#include <Renderer/Components/CameraComponent.h>
#include <Renderer/MeshGenerator.h>
#include <Random/RandomGenerator.h>
#include <Utils/Utils.h>

#include "OrbitCamera.h"

using namespace Workbench;

class RigidBodyTest : public Workbench::Layer {
public:
	RigidBodyTest(Workbench::Engine* engine) : m_Engine(engine) {};

	void createRigidCube(const Workbench::UUID* cube, mathfu::vec3 pos) {
		world->CreateComponentForEntity<TransformComponent>(cube, pos, mathfu::vec3(M_PI * 0.2f, 0.0f, M_PI * 0.1f));
		auto rigid = world->CreateComponentForEntity<RigidBodyComponent>(cube, 1.0f);
		rigid->linear_acceleration = { 0.0f, -9.81f, 0.0f };
		world->CreateComponentForEntity<MeshComponent>(cube, MeshGenerator::CreateBox());
	}

	void buildCubes() {
		plane = world->CreateEntity();
		auto collider = new HalfSpaceCollider({ 0.0f, 1.0f, 0.0f }, 0.0f);
		auto _plane = world->CreateComponentForEntity<RigidBodyComponent>(plane, 1.0f, mathfu::vec3(0.0f, 0.0f, 0.0f), mathfu::vec3(0.0f, 0.0f, 0.0f), collider);
		_plane->physicsEnabled = false;
		world->CreateComponentForEntity<TransformComponent>(plane);
		world->CreateComponentForEntity<MeshComponent>(plane, MeshGenerator::CreatePlane(5));

		cube1 = world->CreateEntity();
		cube2 = world->CreateEntity();

		createRigidCube(cube1, { 0.0f, 3.5f, 0.0f });
		createRigidCube(cube2, { 0.5f, 6.5f, 0.0f });
		
		//auto rigid = world->GetEntityComponent<RigidBodyComponent>(cube1);
		//rigid->linear_acceleration = { 0.0f, 0.0f, 0.0f };
	}

	void destroyCubes() {
		world->DestroyEntity(cube1);
		world->DestroyEntity(cube2);
		world->DestroyEntity(plane);
	}

	void OnAttach() override {

		BIND_EVENT(this, RigidBodyTest::OnButtonPressed);

		world = ECS::GetInstance();

		m_camera = new OrbitCamera(m_Engine);

		buildCubes();
	}

	void OnUpdate(WB_GAME_TIMER* timer) override {
		if (cube1 != nullptr) {
			auto transform = world->GetEntityComponent<TransformComponent>(cube1);

			if (transform->position.y < -3.0f || transform->position.y > 4.0f) {
				destroyCubes();
				buildCubes();
				return;
			}
		}
	}

	void OnButtonPressed(const Event<Window::Events>* event) {
		if (event->getType() == Window::Events::WindowButtonPressedEvent) {
			auto casted_event = static_cast<const Window::WindowButtonPressedEvent*>(event);
			if (casted_event->getButton() == Workbench::WB_KEYCODES::WB_VK_G) {
				destroyCubes();
				buildCubes();
			}
		}
	};
protected:
	OrbitCamera* m_camera = nullptr;

	ECS* world = nullptr;

	const Workbench::UUID* cube1 = nullptr;
	const Workbench::UUID* cube2 = nullptr;
	const Workbench::UUID* plane = nullptr;

	Workbench::Engine* m_Engine = nullptr;
};

