#pragma once
#include <Layers/Layer.h>
#include <Physics/TransformComponent.h>
#include <Physics/PhysicsComponent.h>

class ExampleLayer : public Workbench::Layer {
public:
	ExampleLayer(Workbench::Engine* engine) : m_Engine(engine) {};

	void OnAttach() override {
		auto entity1 = m_Engine->GetWorld()->CreateEntity();
		auto entity2 = m_Engine->GetWorld()->CreateEntity();

		auto m_component1 = new Workbench::TransformComponent(entity1, mathfu::vec4(0, 0, 0, 0), mathfu::vec4(10, 15, 5, 0));
		auto m_component2 = new Workbench::TransformComponent(entity2);

		auto m_component3 = new Workbench::PhysicsComponent(entity1, 10, mathfu::vec4(0, 0, 0, 0), mathfu::vec4(0, 0, 0, 0));
		auto m_component4 = new Workbench::PhysicsComponent(entity2, 10, mathfu::vec4(0, 0, 0, 0), mathfu::vec4(0, 0, 0, 0));

		m_Engine->GetWorld()->AddComponent(entity1, m_component1);
		m_Engine->GetWorld()->AddComponent(entity2, m_component2);
		m_Engine->GetWorld()->AddComponent(entity1, m_component3);
		m_Engine->GetWorld()->AddComponent(entity2, m_component4);
	}
protected:
	Workbench::Engine* m_Engine = nullptr;
};