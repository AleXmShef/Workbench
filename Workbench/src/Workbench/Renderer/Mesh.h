#pragma once
#include "wbpch.h"

namespace Workbench {
	struct VertexColoredBasic {
		mathfu::VectorPacked<float, 3> Pos;
		mathfu::VectorPacked<float, 4> Color;
	};

	template<class VertexType>
	struct Mesh {
		std::vector<VertexType> m_Verticies;
		std::vector<int> m_Indicies;
	};
}

