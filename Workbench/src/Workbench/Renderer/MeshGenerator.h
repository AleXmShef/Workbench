#pragma once
#include "wbpch.h"
#include "Core.h"
#include "Renderer/Primitives/Mesh.h"

namespace Workbench {
	class WORKBENCH_API MeshGenerator {
	public:
		static Mesh* CreateBox(float height = 1, mathfu::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f});
		static Mesh* CreateBox(mathfu::vec3 dimensions, mathfu::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f });
		static Mesh* CreateTriangle(float length = 1);
		static Mesh* CreatePlane(float width = 1);
	};
}
