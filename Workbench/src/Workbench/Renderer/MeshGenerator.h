#pragma once
#include "wbpch.h"
#include "Core.h"
#include "Renderer/Primitives/Mesh.h"

namespace Workbench {
	class WORKBENCH_API MeshGenerator {
	public:
		static Mesh* CreateBox(float height = 1);
		static Mesh* CreateTriangle(float length = 1);
	};
}
