#pragma once

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

class ParabolicTrajectory : public Workbench::Layer {
public:
	ParabolicTrajectory(Workbench::Engine* engine) : m_Engine(engine) {};

	void OnKeyboardPressed(const Workbench::Event<Workbench::Window::Events>* event) {
		if (event->getType() == Workbench::Window::Events::WindowButtonPressedEvent) {
			auto _event = dynamic_cast<const Workbench::Window::WindowButtonPressedEvent*>(event);
			if (_event->getButton() == Workbench::WB_KEYCODES::WB_VK_G) {
				WB_LOG("Creating new entity!");

				auto newCube = m_Engine->GetWorld()->CreateEntity();

				auto entityTransform = new Workbench::TransformComponent(newCube, {0.0f, 0.0f, 0.0f});
				auto rigidBodyComponent = new Workbench::RigidBodyComponent(newCube, 1.0f, {10.0f, 10.0f, 0.0f});
				mathfu::vec4 randomColor = { (float)RandomGenerator::random_in_range(0, 10) / 10, (float)RandomGenerator::random_in_range(0, 10) / 10, (float)RandomGenerator::random_in_range(0, 10) / 10 , 1.0f };
				auto mesh = new Workbench::MeshComponent(newCube, Workbench::MeshGenerator::CreateBox(1.0f, randomColor));

				m_Engine->GetWorld()->AddComponent(newCube, entityTransform);
				m_Engine->GetWorld()->AddComponent(newCube, mesh);
				m_Engine->GetWorld()->AddComponent(newCube, rigidBodyComponent);
			}
		}
		else if (event->getType() == Workbench::Window::Events::WindowMouseButtonPressedEvent) {
			auto _event = dynamic_cast<const Workbench::Window::WindowMouseButtonPressedEvent*>(event);
			if (_event->getButton() == Workbench::WB_KEYCODES::LMB) {
				WB_LOG("Creating new entity!");

				auto newCube = m_Engine->GetWorld()->CreateEntity();

				auto entityTransform = new Workbench::TransformComponent(newCube);
				auto rigidBodyComponent = new Workbench::RigidBodyComponent(newCube, 1.0f, { 10.0f, 10.0f, 0.0f});
				mathfu::vec4 randomColor = { (float)RandomGenerator::random_in_range(0, 10) / 10, (float)RandomGenerator::random_in_range(0, 10) / 10, (float)RandomGenerator::random_in_range(0, 10) / 10 , 1.0f };
				auto mesh = new Workbench::MeshComponent(newCube, Workbench::MeshGenerator::CreateBox(1.0f, randomColor));

				m_Engine->GetWorld()->AddComponent(newCube, entityTransform);
				m_Engine->GetWorld()->AddComponent(newCube, mesh);
				m_Engine->GetWorld()->AddComponent(newCube, rigidBodyComponent);
			}
		}
	};

	void OnAttach() override {
		BIND_EVENT(this, ParabolicTrajectory::OnKeyboardPressed);

		m_camera = new OrbitCamera(m_Engine);

		auto entity1 = m_Engine->GetWorld()->CreateEntity();

		auto m_component1 = new Workbench::TransformComponent(entity1);
		//auto m_component2 = new Workbench::MeshComponent(entity1, Workbench::MeshGenerator::CreateBox());

		m_Engine->GetWorld()->AddComponent(entity1, m_component1);
		//m_Engine->GetWorld()->AddComponent(entity1, m_component2);
		m_Engine->GetWorld()->CreateComponentForEntity<MeshComponent>(entity1, Workbench::MeshGenerator::CreateBox());
	}

	void OnUpdate(WB_GAME_TIMER* timer) override {
		auto components = ECS::GetInstance()->GetComponents<TransformComponent>();
		for (auto transform : components) {
			auto camera = ECS::GetInstance()->GetEntityComponent<CameraComponent>(transform->getEntityId());
		
			if (camera == nullptr && transform->position.y < -5.0f) {
				WB_LOG("Deleting entity: out of bounds");
				ECS::GetInstance()->DestroyEntity(transform->getEntityId());
			}
		}
	}
protected:
	OrbitCamera* m_camera = nullptr;

	Workbench::Engine* m_Engine = nullptr;
};
