#pragma once
#include <Workbench.h>

#include <Layers/Layer.h>
#include <Physics/TransformComponent.h>
#include <Physics/PhysicsComponent.h>
#include <Renderer/Components/MeshComponent.h>
#include <Renderer/Components/CameraComponent.h>
#include <Renderer/MeshGenerator.h>
#include <Random/RandomGenerator.h>
#include <Events/EventBus.h>
#include <Events/CallbackHandler.h>
#include <DirectXMath.h>
#include <Window.h>

using namespace Workbench;

float deg2rad(float deg) {
	return deg * (M_PI / 180);
}

class OrbitCamera {
public:
	OrbitCamera(Engine* engine): m_Engine(engine) {
		camera = m_Engine->GetWorld()->CreateEntity();
		auto camera_component = new CameraComponent(camera, 1.0f, (16.0f / 9.0f), { 0.1f, 1000.0f });

		auto x = r * sinf(mPhi) * cosf(mTheta);
		auto z = r * sinf(mPhi) * sinf(mTheta);
		auto y = r * cosf(mPhi);

		auto transform_component = new TransformComponent(camera, { x, y, z, 0.0f });

		m_Engine->GetWorld()->AddComponent(camera, transform_component);
		m_Engine->GetWorld()->AddComponent(camera, camera_component);

		BIND_EVENT(this, OrbitCamera::OnMouseMoved);
	}

protected:
	void OnMouseMoved(const Workbench::Event<Workbench::Window::Events>* event) {
		if (event->getType() == Window::Events::WindowMouseButtonPressedEvent) {
			auto _event = dynamic_cast<const Window::WindowMouseButtonPressedEvent*>(event);

			if (_event->getButton() == WB_KEYCODES::RMB) {
				captured = true;
			}
		}
		else if (event->getType() == Window::Events::WindowMouseButtonReleasedEvent) {
			auto _event = dynamic_cast<const Window::WindowMouseButtonReleasedEvent*>(event);

			if (_event->getButton() == WB_KEYCODES::RMB) {
				captured = false;
			}
		}
		else if (event->getType() == Window::Events::WindowMouseMovedEvent) {
			auto _event = dynamic_cast<const Window::WindowMouseMovedEvent*>(event);

			auto [x, y] = _event->getCoords();

			if (captured) {
				float dx = deg2rad(0.25f * (x - last_x));
				float dy = deg2rad(0.25f * (y - last_y));

				// Update angles based on input to orbit camera around box.
				mTheta -= dx;
				mPhi -= dy;

				updateTransform();
			}

			//else {
				last_x = x;
				last_y = y;
			//}
		}

		else if (event->getType() == Window::Events::WindowMouseWheelRotatedEvent) {
			auto _event = dynamic_cast<const Window::WindowMouseWheelRotatedEvent*>(event);

			auto delta = _event->getDelta();

			r -= 0.01f * delta;

			updateTransform();
		}
	}

	void updateTransform() {
		auto x = r * sinf(mPhi) * cosf(mTheta);
		auto z = r * sinf(mPhi) * sinf(mTheta);
		auto y = r * cosf(mPhi);

		auto transform = ECS::getInstance()->GetEntityComponent<TransformComponent>(camera);
		transform->position = { x, y, z, 0.0f };
		transform->rebuildWorldMatrix();
	}

	bool captured = false;
	
	float last_x = 0;
	float last_y = 0;

	float mPhi = 0.25f * M_PI;
	float mTheta = 0.25f * M_PI;
	float r = 4;

	Engine* m_Engine;
	const Workbench::UUID* camera;

};

class ExampleLayer : public Workbench::Layer {
public:
	ExampleLayer(Workbench::Engine* engine) : m_Engine(engine) {};

	void OnKeyboardPressed(const Workbench::Event<Workbench::Window::Events>* event) {
		if (event->getType() == Workbench::Window::Events::WindowButtonPressedEvent) {
			auto _event = dynamic_cast<const Workbench::Window::WindowButtonPressedEvent*>(event);
			if (_event->getButton() == Workbench::WB_KEYCODES::WB_VK_G) {
				WB_LOG("Creating new entity!");

				auto newCube = m_Engine->GetWorld()->CreateEntity();

				int posX = Workbench::RandomGenerator::random_in_range(-5, 5);
				int posY = Workbench::RandomGenerator::random_in_range(-5, 5);
				int posZ = Workbench::RandomGenerator::random_in_range(-5, 5);

				auto entityTransform = new Workbench::TransformComponent(newCube, { (float)posX, (float)posY, (float)posX, 0.0f });
				auto mesh = new Workbench::MeshComponent(newCube, Workbench::MeshGenerator::CreateBox());

				m_Engine->GetWorld()->AddComponent(newCube, entityTransform);
				m_Engine->GetWorld()->AddComponent(newCube, mesh);
			}
		}
	};

	void OnAttach() override {
		BIND_EVENT(this, ExampleLayer::OnKeyboardPressed);

		auto entity1 = m_Engine->GetWorld()->CreateEntity();
		//auto camera = m_Engine->GetWorld()->CreateEntity();
		//
		//m_camera = camera;

		m_camera = new OrbitCamera(m_Engine);

		auto m_component1 = new Workbench::TransformComponent(entity1);
		auto m_component2 = new Workbench::MeshComponent(entity1, Workbench::MeshGenerator::CreateBox());

		m_Engine->GetWorld()->AddComponent(entity1, m_component1);
		m_Engine->GetWorld()->AddComponent(entity1, m_component2);

		//auto m_component3 = new Workbench::TransformComponent(camera, mathfu::vec4(0, 0, 4, 0));
		//auto m_component4 = new Workbench::CameraComponent(camera, 1.0f, (16.0f/9.0f),  {1.0f, 1000.0f});
		//
		//m_Engine->GetWorld()->AddComponent(camera, m_component3);
		//m_Engine->GetWorld()->AddComponent(camera, m_component4);
	}
protected:
	OrbitCamera* m_camera = nullptr;

	Workbench::Engine* m_Engine = nullptr;
};