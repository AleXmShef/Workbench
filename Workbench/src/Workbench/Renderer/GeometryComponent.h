#pragma once
#include "wbpch.h"
#include "ECS/ECSComponent.h"
#include "Renderer/Mesh.h"

namespace Workbench {
	struct GeometryComponent : public ECSComponent {
	public:
		GeometryComponent(const UUID* entity, Mesh* mesh) : ECSComponent(entity) {};

		Mesh* GetMesh();
	protected:
		Mesh* m_Mesh;
	};
}

