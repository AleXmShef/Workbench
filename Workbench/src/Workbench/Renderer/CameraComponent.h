#pragma once
#include "wbpch.h"
#include "ECS/ECSComponent.h"

namespace Workbench {
	struct CameraComponent : public ECSComponent {
		CameraComponent
		(
			const UUID* entity,
			std::pair<float, float> _fov, 
			std::pair<float, float> _planes
		) : ECSComponent(entity), fov(_fov), planes(_planes) 
		{
			projMatrix = mathfu::mat4::Perspective(fov.second, fov.first / fov.second, planes.first, planes.second, -1.0f);
		};

		mathfu::mat4 viewMatrix;
		mathfu::mat4 projMatrix;

		std::pair<float, float> fov;
		std::pair<float, float> planes;
	};
}
