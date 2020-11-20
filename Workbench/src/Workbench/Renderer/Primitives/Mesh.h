#pragma once
#include "wbpch.h"

namespace Workbench {
	struct VertexColoredBasic {
		mathfu::VectorPacked<float, 3> Pos;
		mathfu::VectorPacked<float, 4> Color;
	};

	struct Mesh {
	public:
		Mesh(
			std::vector<VertexColoredBasic> verts, 
			std::vector<uint32_t> inds, 
			std::string name = "<undefined>"
		) : Name(name), 
			Verticies(verts), 
			Indicies(inds) 
		{};

		std::string Name;

		std::vector<VertexColoredBasic> Verticies;
		std::vector<uint32_t> Indicies;
	};
}

