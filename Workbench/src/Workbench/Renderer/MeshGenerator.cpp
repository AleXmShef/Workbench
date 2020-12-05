#include "wbpch.h"
#include "Renderer/MeshGenerator.h"

namespace Workbench {
	Mesh* MeshGenerator::CreateBox(float height) {
		auto mesh = new Mesh();

		VertexColoredBasic vert1;
		vert1.Pos = {-0.5f*height, -0.5f*height, -0.5f*height};
		vert1.Color = {1.0f, 0.0f, 0.0f, 1.0f};

		VertexColoredBasic vert2;
		vert2.Pos = { -0.5f * height, 0.5f * height, -0.5f * height };
		vert2.Color = { 1.0f, 0.0f, 0.0f, 1.0f };

		VertexColoredBasic vert3;
		vert3.Pos = { 0.5f * height, 0.5f * height, -0.5f * height };
		vert3.Color = { 1.0f, 0.0f, 0.0f, 1.0f };

		VertexColoredBasic vert4;
		vert4.Pos = { 0.5f * height, -0.5f * height, -0.5f * height };
		vert4.Color = { 1.0f, 0.0f, 0.0f, 1.0f };

		VertexColoredBasic vert5;
		vert5.Pos = { -0.5f * height, -0.5f * height, 0.5f * height };
		vert5.Color = { 1.0f, 0.0f, 0.0f, 1.0f };

		VertexColoredBasic vert6;
		vert6.Pos = { -0.5f * height, 0.5f * height, 0.5f * height };
		vert6.Color = { 1.0f, 0.0f, 0.0f, 1.0f };

		VertexColoredBasic vert7;
		vert7.Pos = { 0.5f * height, 0.5f * height, 0.5f * height };
		vert7.Color = { 1.0f, 0.0f, 0.0f, 1.0f };
		
		VertexColoredBasic vert8;
		vert8.Pos = { 0.5f * height, -0.5f * height, 0.5f * height };
		vert8.Color = { 1.0f, 0.0f, 0.0f, 1.0f };


		mesh->Verticies.push_back(vert1);
		mesh->Verticies.push_back(vert2);
		mesh->Verticies.push_back(vert3);
		mesh->Verticies.push_back(vert4);
		mesh->Verticies.push_back(vert5);
		mesh->Verticies.push_back(vert6);
		mesh->Verticies.push_back(vert7);
		mesh->Verticies.push_back(vert8);
		

		//mesh->Verticies.push_back({mathfu::vec3_packed({-0.5f*height, -0.5f*height, -0.5*height}), mathfu::vec4_packed({1.0f,1.0f,1.0f,1.0f})});
		//VertexColoredBasic({mathfu::vec3_packed({-0.5f*height, +0.5f*height, -0.5*height}), mathfu::vec4_packed({1.0f,1.0f,1.0f,1.0f})});
		//VertexColoredBasic({mathfu::vec3_packed({+0.5f*height, +0.5f*height, -0.5*height}), mathfu::vec4_packed({1.0f,1.0f,1.0f,1.0f})});
		//VertexColoredBasic({mathfu::vec3_packed({+0.5f*height, -0.5f*height, -0.5*height}), mathfu::vec4_packed({1.0f,1.0f,1.0f,1.0f})});
		//VertexColoredBasic({mathfu::vec3_packed({-0.5f*height, -0.5f*height, +0.5*height}), mathfu::vec4_packed({1.0f,1.0f,1.0f,1.0f})});
		//VertexColoredBasic({mathfu::vec3_packed({-0.5f*height, +0.5f*height, +0.5*height}), mathfu::vec4_packed({1.0f,1.0f,1.0f,1.0f})});
		//VertexColoredBasic({mathfu::vec3_packed({+0.5f*height, +0.5f*height, +0.5*height}), mathfu::vec4_packed({1.0f,1.0f,1.0f,1.0f})});
		//VertexColoredBasic({mathfu::vec3_packed({+0.5f*height, -0.5f*height, +0.5*height}), mathfu::vec4_packed({1.0f,1.0f,1.0f,1.0f})});

		mesh->Indicies = {
			// front face
			0, 1, 2,
			0, 2, 3,
		
			// back face
			4, 6, 5,
			4, 7, 6,
		
			// left face
			4, 5, 1,
			4, 1, 0,
		
			// right face
			3, 2, 6,
			3, 6, 7,
		
			// top face
			1, 5, 6,
			1, 6, 2,
		
			// bottom face
			4, 0, 3,
			4, 3, 7
		};

		mesh->Name = "BoxGeneric";

		return mesh;
	}

	Mesh* MeshGenerator::CreateTriangle(float length) {
		auto mesh = new Mesh();

		VertexColoredBasic vert1;
		vert1.Pos = {-0.5f, 0.0f, 0.0f};
		vert1.Color = { 1.0f, 0.0f, 0.0f, 1.0f };

		VertexColoredBasic vert2;
		vert2.Pos = { 0.0f, 1.0f, 0.0f };
		vert2.Color = { 1.0f, 0.0f, 0.0f, 1.0f };

		VertexColoredBasic vert3;
		vert3.Pos = { 0.5f, 0.0f, 0.0f };
		vert3.Color = { 1.0f, 0.0f, 0.0f, 1.0f };

		mesh->Verticies.push_back(vert1);
		mesh->Verticies.push_back(vert2);
		mesh->Verticies.push_back(vert3);

		mesh->Indicies = {
			0, 1, 2
		};

		mesh->Name = "Triangle";

		return mesh;
	}
}