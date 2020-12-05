#include "wbpch.h"
#include "RenderSystem.h"
#include "Components/MeshComponent.h"
#include "Components/CameraComponent.h"
#include "Physics/TransformComponent.h"
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
			UpdateObjects();
			if (m_MeshResources[0]->IsDirty())
				m_MeshResources[0]->UploadResource();
			m_Renderer->DrawMeshes(m_MeshResources[0]);
		}
		m_Renderer->End();
	}

	void RenderSystem::UpdateObjects() {
		auto camera = ECS::getInstance()->GetEntityComponent<CameraComponent>(m_ActiveCamera);
		auto camera_pos = ECS::getInstance()->GetEntityComponent<TransformComponent>(m_ActiveCamera);
		
		camera->viewMatrix = mathfu::Matrix<float, 4, 4>::LookAt(
			mathfu::vec3(0.0f, 0.0f, 0.0f),
			camera_pos->position.xyz(),
			mathfu::vec3(0, 1, 0),
			WB_HANDEDNESS == "right" ? 1.0f : -1.0f
		);

		WB_RENDERER_INFO("Camera position: {}", camera_pos->position);

		DirectX::XMVECTOR pos = DirectX::XMVectorSet(camera_pos->position.x, camera_pos->position.y, camera_pos->position.z, 0.0f);
		DirectX::XMVECTOR target = DirectX::XMVectorZero();
		DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		DirectX::XMMATRIX _viewMatrix = DirectX::XMMatrixLookAtLH(pos, target, up);

		WB_RENDERER_INFO("DirectX view matrix: {}", _viewMatrix);

		WB_RENDERER_INFO("Mathfu view matrix: {}", camera->viewMatrix);
		
		
		
		auto submeshes = m_MeshResources[0]->GetSubmeshes();
		
		ObjectConstants* constants = new ObjectConstants[submeshes->size()];
		
		int i = 0;
		for (auto it = submeshes->begin(); it != submeshes->end(); ++it) {
			auto something = *it;
			const UUID* entity = something.first;
			Workbench::MeshResource::SubMesh& submesh = something.second;
			auto entity_pos = ECS::getInstance()->GetEntityComponent<TransformComponent>(entity);

			//WB_RENDERER_LOG("Object world matrix: {}", entity_pos->worldMatrix);
		
			mathfu::mat4 worldViewProj = entity_pos->worldMatrix * camera->viewMatrix * camera->projMatrix;

			auto verticies = m_MeshResources[0]->GetVertexArray();
			mathfu::vec3 __vec((*verticies)[0].Pos);
			auto aftPos = mathfu::vec4(__vec, 1.0f) * worldViewProj;

			WB_CORE_INFO("Vertex position: {}", aftPos);
		
			ObjectConstants _constants;
			
			worldViewProj.Transpose().Pack(_constants.WorldViewProj);
		
			constants[i] = _constants;
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
			auto _event = dynamic_cast<const ECS::EntityComponentsChangedEvent<MeshComponent>*>(event);
			if (_event) {
				if (_event->getActionType() == ECS::EntityComponentsChangedEvent<MeshComponent>::ActionType::ComponentCreated) {
					auto component = _event->getComponent();
					m_MeshResources[0]->AddMesh(component->getEntityId(), component->GetMesh());
					m_MeshResources[0]->ReleaseResource();
				}
			}
			auto event_ = dynamic_cast<const ECS::EntityComponentsChangedEvent<CameraComponent>*>(event);
			if (event_) {
				m_ActiveCamera = event_->getComponent()->getEntityId();
			}
		}
	}

	void RenderSystem::SetActiveCamera(const UUID* camera) {
		if (ECS::getInstance()->GetEntityComponent<CameraComponent>(camera) != nullptr)
			m_ActiveCamera = camera;
		else
			WB_RENDERER_ERROR("Tried to set entity {0} as active camera, but this enity doesn't have CameraComponent!", camera);
	}
}
