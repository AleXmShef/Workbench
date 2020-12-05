#pragma once
#include <Workbench.h>

#include <Layers/Layer.h>
#include <Physics/TransformComponent.h>
#include <Physics/PhysicsComponent.h>
#include <Renderer/Components/MeshComponent.h>
#include <Renderer/Components/CameraComponent.h>
#include <Renderer/MeshGenerator.h>

class ExampleLayer : public Workbench::Layer {
public:
	ExampleLayer(Workbench::Engine* engine) : m_Engine(engine) {};

	void OnAttach() override {
		auto entity1 = m_Engine->GetWorld()->CreateEntity();
		auto entity2 = m_Engine->GetWorld()->CreateEntity();

		auto m_component1 = new Workbench::TransformComponent(entity1);
		auto m_component2 = new Workbench::MeshComponent(entity1, Workbench::MeshGenerator::CreateTriangle());

		m_Engine->GetWorld()->AddComponent(entity1, m_component1);
		m_Engine->GetWorld()->AddComponent(entity1, m_component2);

		auto m_component3 = new Workbench::TransformComponent(entity2, mathfu::vec4(0, 0, 10, 0));
		auto m_component4 = new Workbench::CameraComponent(entity2, { 0.785f, 1.0f }, {0.1f, 1000.0f});

		m_Engine->GetWorld()->AddComponent(entity2, m_component3);
		m_Engine->GetWorld()->AddComponent(entity2, m_component4);
	}
protected:
	Workbench::Engine* m_Engine = nullptr;
};