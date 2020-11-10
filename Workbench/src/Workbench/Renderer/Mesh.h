#pragma once
#include "wbpch.h"

namespace Workbench {
	struct Vertex {
		mathfu::vec4 pos;
		mathfu::vec4 color;
	};
	struct Mesh {
		std::vector<Vertex> m_Verticies;
		std::vector<int> m_Indicies;
	};
}

