#pragma once
#include "wbpch.h"
#include "ECS/ECSComponent.h"
#include "Renderer/Primitives/Mesh.h"

namespace Workbench {
	struct MeshComponent : public ECSComponent {
	public:
		MeshComponent(const UUID* entity, Mesh* mesh) : ECSComponent(entity), m_Mesh(mesh) {};
		
		Mesh* GetMesh() { return m_Mesh; }
	protected:
		Mesh* m_Mesh;
	};
}

