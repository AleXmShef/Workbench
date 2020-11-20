#pragma once
#include "wbpch.h"
#include "ECS/ECSComponent.h"

namespace Workbench {
	struct MeshComponent : public ECSComponent {
	public:
		MeshComponent(const UUID* entity, std::string& mesh) : ECSComponent(entity) {};
		
		std::string GetMeshName() { return m_MeshName; }
	protected:
		std::string m_MeshName;
	};
}

