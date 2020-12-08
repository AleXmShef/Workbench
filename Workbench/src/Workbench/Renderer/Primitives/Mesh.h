#pragma once
#pragma warning(disable : 4251)
#include "wbpch.h"
#include "Core.h"

namespace Workbench {
	struct WORKBENCH_API VertexColoredBasic {
		VertexColoredBasic() = default;
		VertexColoredBasic(mathfu::vec3 pos, mathfu::vec4 color) : Pos(pos), Color(color) {};

		mathfu::vec3_packed Pos;
		mathfu::vec4_packed Color;
	};

	struct ObjectConstants {
		mathfu::vec4_packed WorldViewProj[4];
		mathfu::vec4_packed Color;
	};

	struct WORKBENCH_API Mesh {
	public:
		Mesh() {};
		Mesh(
			std::vector<VertexColoredBasic> verts, 
			std::vector<uint32_t> inds, 
			std::string name = "<undefined>"
		) : Name(name), 
			Verticies(verts), 
			Indicies(inds) 
		{};

		std::string Name = "<undefined>";
		mathfu::vec4 Color;

		std::vector<VertexColoredBasic> Verticies;
		std::vector<uint32_t> Indicies;
	};
}

