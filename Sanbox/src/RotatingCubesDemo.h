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

class RotatinCubesDemo : public Workbench::Layer {
public:
	RotatinCubesDemo(Workbench::Engine* engine) : m_Engine(engine) {};

	void OnKeyboardPressed(const Workbench::Event<Workbench::Window::Events>* event) {
		if (event->getType() == Workbench::Window::Events::WindowButtonPressedEvent) {
			auto _event = dynamic_cast<const Workbench::Window::WindowButtonPressedEvent*>(event);
			if (_event->getButton() == Workbench::WB_KEYCODES::WB_VK_G) {
				WB_LOG("Creating new entity!");

				auto newCube = m_Engine->GetWorld()->CreateEntity();

				int posX = Workbench::RandomGenerator::random_in_range(-5, 5);
				int posY = Workbench::RandomGenerator::random_in_range(-5, 5);
				int posZ = Workbench::RandomGenerator::random_in_range(-5, 5);

				auto entityTransform = new Workbench::TransformComponent(newCube, { (float)posX, (float)posY, (float)posZ, 0.0f });
				auto mesh = new Workbench::MeshComponent(newCube, Workbench::MeshGenerator::CreateBox());

				m_Engine->GetWorld()->AddComponent(newCube, entityTransform);
				m_Engine->GetWorld()->AddComponent(newCube, mesh);
			}
		}
	};

	void OnAttach() override {
		BIND_EVENT(this, RotatinCubesDemo::OnKeyboardPressed);

		auto entity1 = m_Engine->GetWorld()->CreateEntity();

		m_camera = new OrbitCamera(m_Engine);

		auto m_component1 = new Workbench::TransformComponent(entity1);
		auto m_component2 = new Workbench::MeshComponent(entity1, Workbench::MeshGenerator::CreateBox());

		m_Engine->GetWorld()->AddComponent(entity1, m_component1);
		m_Engine->GetWorld()->AddComponent(entity1, m_component2);
	}

	void OnUpdate(WB_GAME_TIMER* timer) override {
		auto components = ECS::GetInstance()->GetComponents<TransformComponent>();
		for (auto transform : components) {
			auto camera = ECS::GetInstance()->GetEntityComponent<CameraComponent>(transform->getEntityId());

			auto test = timer->GetTickTime();

			if (camera == nullptr) {
				transform->rotation = mathfu::vec4(
					transform->rotation.x,
					transform->rotation.y + M_PI / 2 * timer->GetTickTime(),
					transform->rotation.z,
					transform->rotation.w
				);

				transform->rebuildWorldMatrix();
			}
		}
	}
protected:
	OrbitCamera* m_camera = nullptr;

	Workbench::Engine* m_Engine = nullptr;
};