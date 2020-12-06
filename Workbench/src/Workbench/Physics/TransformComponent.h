#pragma once
#include "wbpch.h"
#include "ECS/ECSComponent.h"
#include "Logger/Logger.h"

namespace Workbench {
	struct TransformComponent : public ECSComponent {
		TransformComponent(
			const UUID* entity,
			mathfu::vec4 _position = mathfu::vec4(0, 0, 0, 0), 
			mathfu::vec4 _rotation = mathfu::vec4(0, 0, 0, 0),
			mathfu::vec4 _scale	   = mathfu::vec4(1, 1, 1, 0)
		) : ECSComponent(entity), position(_position), rotation(_rotation), scale(_scale) 
		{
			rebuildWorldMatrix();
		};

		void rebuildWorldMatrix() {
			auto rotationMatrix = mathfu::mat3::RotationX(rotation.x) * mathfu::mat3::RotationY(rotation.y) * mathfu::mat3::RotationZ(rotation.z);
			worldMatrix =
				mathfu::mat4::FromScaleVector(scale.xyz()) *
				mathfu::mat4::FromRotationMatrix(rotationMatrix) *
				mathfu::mat4::FromTranslationVector(position.xyz());
				
		}

		mathfu::vec4 position;
		mathfu::vec4 rotation;
		mathfu::vec4 scale;

		mathfu::mat4 worldMatrix;
	};
}
