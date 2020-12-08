#include "wbpch.h"
#include "RenderSystem.h"
#include "Components/MeshComponent.h"
#include "Components/CameraComponent.h"
#include "Physics/Components/TransformComponent.h"
#include "DirectXMath.h"

namespace Workbench {

	RenderSystem::RenderSystem(std::shared_ptr<Window> window) {

		m_Renderer = WB_CREATE_RENDERER();
		m_Window = window;
	}

	void RenderSystem::OnAttach() {
		m_Renderer->Init(m_Window);

		BIND_EVENT(this, RenderSystem::OnMeshComponentChanged);

		auto VertexBuffer = m_Renderer->CreateConstantResource();
		auto IndexBuffer = m_Renderer->CreateConstantResource();

		auto ConstantBuffer = m_Renderer->CreateDynamicResource(true);
		
		auto meshResource = new MeshResource(VertexBuffer, IndexBuffer, ConstantBuffer);

		m_MeshResources.push_back(meshResource);
	}

	void RenderSystem::OnUpdate(WB_GAME_TIMER* timer) {
		m_Renderer->Begin();
		if (m_ActiveCamera != nullptr) {
			if (m_MeshResources[0]->IsDirty())
				m_MeshResources[0]->UploadResource();
			UpdateObjects();
			m_Renderer->DrawMeshes(m_MeshResources[0]);
		}
		m_Renderer->End();
	}

	void RenderSystem::UpdateObjects() {
		auto camera = ECS::GetInstance()->GetEntityComponent<CameraComponent>(m_ActiveCamera);
		auto camera_pos = ECS::GetInstance()->GetEntityComponent<TransformComponent>(m_ActiveCamera);
		
		camera->viewMatrix = mathfu::mat4::LookAt(
			mathfu::vec3(0.0f, 0.0f, 0.0f),
			camera_pos->position.xyz(),
			mathfu::vec3(0, 1, 0),
			-1.0f
		);

		auto submeshes = m_MeshResources[0]->GetSubmeshes();
		
		ObjectConstants* constants = new ObjectConstants[submeshes->size()];
		
		int i = 0;
		for (auto it = submeshes->begin(); it != submeshes->end(); ++it) {
			auto something = *it;
			const UUID* entity = something.first;
			Workbench::MeshResource::SubMesh& submesh = something.second;
			auto entity_pos = ECS::GetInstance()->GetEntityComponent<TransformComponent>(entity);

			mathfu::mat4 worldViewProj = (camera->projMatrix * camera->viewMatrix * entity_pos->worldMatrix).Transpose();

			worldViewProj.Pack(constants[i].WorldViewProj);

			auto mesh_comp = ECS::GetInstance()->GetEntityComponent<MeshComponent>(entity);
			constants[i].Color = mesh_comp->GetMesh()->Color;

			++i;
		}
		
		m_MeshResources[0]->GetConstantBuffer()->UpdateResource(
			(const void*)constants,
			sizeof(ObjectConstants) * submeshes->size(),
			sizeof(ObjectConstants),
			submeshes->size()
		);
	}

	void RenderSystem::OnDetach() {
		delete m_MeshResources[0];
		delete m_Renderer;
	}

	void RenderSystem::OnMeshComponentChanged(const Event<ECS::Events>* event) {
		if (event->getType() == ECS::Events::EntityComponentsChangedEvent) {
			auto _event = dynamic_cast<const ECS::EntityComponentsChangedEvent*>(event);
			if (_event && _event->GetComponentType() == typeid(MeshComponent)) {
				if (_event->GetActionType() == ECS::EntityComponentsChangedEvent::ActionType::ComponentCreated) {
					auto component = static_cast<MeshComponent*>(_event->GetComponent());
					m_MeshResources[0]->AddMesh(component->getEntityId(), component->GetMesh());
					m_MeshResources[0]->ReleaseResource();
				}
				else if (_event->GetActionType() == ECS::EntityComponentsChangedEvent::ActionType::ComponentDestroyed) {
					auto component = _event->GetComponent();
					m_MeshResources[0]->RemoveMesh(component->getEntityId());
					m_MeshResources[0]->ReleaseResource();
				}
			}
			auto event_ = dynamic_cast<const ECS::EntityComponentsChangedEvent*>(event);
			if (event_ && _event->GetComponentType() == typeid(CameraComponent)) {
				m_ActiveCamera = event_->GetComponent()->getEntityId();
			}
		}
	}

	void RenderSystem::SetActiveCamera(const UUID* camera) {
		if (ECS::GetInstance()->GetEntityComponent<CameraComponent>(camera) != nullptr)
			m_ActiveCamera = camera;
		else
			WB_RENDERER_ERROR("Tried to set entity {0} as active camera, but this enity doesn't have CameraComponent!", camera);
	}
}
