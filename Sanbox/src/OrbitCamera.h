#pragma once
#include <Workbench.h>
#include <Renderer/Components/CameraComponent.h>
#include <Physics/Components/TransformComponent.h>

using namespace Workbench;

class OrbitCamera {
public:
	OrbitCamera(Engine* engine) : m_Engine(engine) {
		camera = m_Engine->GetWorld()->CreateEntity();
		auto camera_component = new CameraComponent(camera, 1.0f, (16.0f / 9.0f), { 0.1f, 1000.0f });

		auto x = r * sinf(mPhi) * cosf(mTheta);
		auto z = r * sinf(mPhi) * sinf(mTheta);
		auto y = r * cosf(mPhi);

		auto transform_component = new TransformComponent(camera, { x, y, z});

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

				mPhi = mathfu::Clamp(mPhi, 0.01f, (float)M_PI - 0.01f);

				updateTransform();
			}

			last_x = (float)x;
			last_y = (float)y;
		}

		else if (event->getType() == Window::Events::WindowMouseWheelRotatedEvent) {
			auto _event = dynamic_cast<const Window::WindowMouseWheelRotatedEvent*>(event);

			auto delta = _event->getDelta();

			r -= 0.01f * delta;

			r = mathfu::Clamp(r, 1.5f, 50.0f);

			updateTransform();
		}
	}

	void updateTransform() {
		auto x = r * sinf(mPhi) * cosf(mTheta);
		auto z = r * sinf(mPhi) * sinf(mTheta);
		auto y = r * cosf(mPhi);

		auto transform = ECS::GetInstance()->GetEntityComponent<TransformComponent>(camera);
		transform->position = { x, y, z, 0.0f };
		transform->rebuildWorldMatrix();
	}

	bool captured = false;

	float last_x = 0;
	float last_y = 0;

	float mPhi = (float)(0.25f * M_PI);
	float mTheta = (float)(0.25f * M_PI);
	float r = 4;

	Engine* m_Engine;
	const Workbench::UUID* camera;

};
