#pragma once
#include "wbpch.h"
#include "Core.h"

namespace Workbench {
	struct WORKBENCH_API VertexColoredBasic {
		mathfu::vec3_packed Pos;
		mathfu::vec4_packed Color;
	};

	struct ObjectConstants {
		mathfu::VectorPacked<float, 4> WorldViewProj[4];
	};

	struct WORKBENCH_API Mesh {
	public:
		Mesh() {};
		Mesh(
			std::vector<VertexColoredBasic> verts, 
			std::vector<uint16_t> inds, 
			std::string name = "<undefined>"
		) : Name(name), 
			Verticies(verts), 
			Indicies(inds) 
		{};

		std::string Name = "<undefined>";

		std::vector<VertexColoredBasic> Verticies;
		std::vector<uint16_t> Indicies;
	};
}

