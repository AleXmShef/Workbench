#pragma once
#include "wbpch.h"
#include "ECS/ECSComponent.h"
#include "DirectXMath.h"

namespace Workbench {
	struct CameraComponent : public ECSComponent {
		CameraComponent
		(
			const UUID* entity,
			std::pair<float, float> _fov, 
			std::pair<float, float> _planes
		) : ECSComponent(entity), fov(_fov), planes(_planes) 
		{
			DirectX::XMMATRIX _projMatrix = DirectX::XMMatrixPerspectiveFovLH(fov.first, fov.second / fov.first, planes.first, planes.second);
			WB_CORE_INFO("DirectX projection matrix: {}", _projMatrix);
			DirectX::XMFLOAT4X4 __projMatrix;
			DirectX::XMStoreFloat4x4(&__projMatrix, _projMatrix);
			projMatrix = mathfu::mat4::Perspective(fov.first, fov.second / fov.first, planes.first, planes.second, -1.0f);
			WB_CORE_INFO("Mathfu projection matrix: {}", projMatrix);
		};

		mathfu::mat4 viewMatrix;
		mathfu::mat4 projMatrix;

		std::pair<float, float> fov;
		std::pair<float, float> planes;
	};
}
