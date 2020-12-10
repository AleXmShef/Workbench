#pragma once
#include "wbpch.h"
#include "Core.h"

namespace Workbench {
	enum class ColliderType {
		BoxCollider = 0
	};

	class Collider3D {
	public:
		Collider3D(mathfu::mat4 _offset = mathfu::mat4::Identity()) : offset(_offset) {};
		virtual ColliderType GetType() = 0;

		mathfu::vec3 GetAxis(int _axis) {
			return mathfu::vec3(transform[0 + 4 * _axis], transform[1 + 4 * _axis], transform[2 + 4 * _axis]);
		}
	public:

		mathfu::mat4 offset;	//local space
		mathfu::mat4 transform;	//world space
	};

	class BoxCollider : public Collider3D {
	public:
		BoxCollider(mathfu::vec3 _dimensions, mathfu::mat4 _offset) : Collider3D(_offset), half_size(_dimensions / 2) {};

		virtual ColliderType GetType() override { return ColliderType::BoxCollider; };
	public:
		mathfu::vec3 half_size;
	};
}

