#pragma once
#include "wbpch.h"
#include "ECS/ECSComponent.h"
#include "DirectXMath.h"

namespace Workbench {
	struct CameraComponent : public ECSComponent {
		CameraComponent
		(
			const UUID* entity,
			float _fov, 
			float _aspect,
			std::pair<float, float> _planes
		) : ECSComponent(entity), yFov(_fov), aspect(_aspect), planes(_planes) 
		{
			projMatrix = mathfu::mat4::Perspective(yFov, aspect, planes.first, planes.second, -1.0f);
		};

		mathfu::mat4 viewMatrix;
		mathfu::mat4 projMatrix;

		float yFov;
		float aspect;
		std::pair<float, float> planes;
	};
}
