#pragma once
#include "wbpch.h"
#include "ECS/ECSComponent.h"
#include "Logger/Logger.h"

namespace Workbench {
	struct TransformComponent : public ECSComponent {
		TransformComponent(
			const UUID* entity,
			mathfu::vec3 _position = mathfu::vec3(0, 0, 0), 
			mathfu::vec3 _rotation = mathfu::vec3(0, 0, 0),
			mathfu::vec3 _scale	   = mathfu::vec3(1, 1, 1)
		) : ECSComponent(entity), position(_position, 0.0f), rotation(mathfu::quat::FromEulerAngles(_rotation)), scale(_scale, 0.0f) 
		{
			rebuildWorldMatrix();
		};

		void rebuildWorldMatrix() {
			worldMatrix = mathfu::mat4::Transform(
				position.xyz(),
				rotation.ToMatrix(),
				scale.xyz());
				
		}

		mathfu::vec4 position;
		mathfu::vec4 scale;
		mathfu::quat rotation;

		mathfu::mat4 worldMatrix;
	};
}
