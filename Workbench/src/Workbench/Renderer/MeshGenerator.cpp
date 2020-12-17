#include "wbpch.h"
#include "Renderer/MeshGenerator.h"

namespace Workbench {
	Mesh* MeshGenerator::CreateBox(float height, mathfu::vec4 color) {
		return CreateBox(mathfu::vec3(height, height, height), color);
	}

	Mesh* MeshGenerator::CreateBox(mathfu::vec3 dimensions, mathfu::vec4 color) {
		auto mesh = new Mesh();

		mesh->Color = color;

		mesh->Verticies = {
			VertexColoredBasic({(float)	-0.5f * dimensions.x,	(float)	-0.5f * dimensions.y, (float)	-0.5 * dimensions.z}, {1.0f,1.0f,1.0f,1.0f}),
			VertexColoredBasic({(float)	-0.5f * dimensions.x,	(float)	 0.5f * dimensions.y, (float)	-0.5 * dimensions.z}, {1.0f,1.0f,1.0f,1.0f}),
			VertexColoredBasic({(float)	 0.5f * dimensions.x,	(float)	 0.5f * dimensions.y, (float)	-0.5 * dimensions.z}, {1.0f,1.0f,1.0f,1.0f}),
			VertexColoredBasic({(float)	 0.5f * dimensions.x,	(float)	-0.5f * dimensions.y, (float)	-0.5 * dimensions.z}, {1.0f,1.0f,1.0f,1.0f}),
			VertexColoredBasic({(float)	-0.5f * dimensions.x,	(float)	-0.5f * dimensions.y, (float)	 0.5 * dimensions.z}, {1.0f,1.0f,1.0f,1.0f}),
			VertexColoredBasic({(float)	-0.5f * dimensions.x,	(float)	 0.5f * dimensions.y, (float)	 0.5 * dimensions.z}, {1.0f,1.0f,1.0f,1.0f}),
			VertexColoredBasic({(float)	 0.5f * dimensions.x,	(float)	 0.5f * dimensions.y, (float)	 0.5 * dimensions.z}, {1.0f,1.0f,1.0f,1.0f}),
			VertexColoredBasic({(float)	 0.5f * dimensions.x,	(float)	-0.5f * dimensions.y, (float)	 0.5 * dimensions.z}, {1.0f,1.0f,1.0f,1.0f})
		};

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
			0, 2, 1
		};

		mesh->Name = "Triangle";

		return mesh;
	}

	Mesh* MeshGenerator::CreatePlane(float width) {
		auto mesh = new Mesh();

		mesh->Verticies = {
			VertexColoredBasic({(float)-0.5f * width, (float)0.0f * width, (float)-0.5 * width}, {1.0f,1.0f,1.0f,1.0f}),
			VertexColoredBasic({(float)0.5f * width, (float)0.0f * width, (float)-0.5 * width}, {1.0f,1.0f,1.0f,1.0f}),
			VertexColoredBasic({(float)-0.5f * width, (float)0.0f * width, (float)0.5 * width}, {1.0f,1.0f,1.0f,1.0f}),
			VertexColoredBasic({(float)0.5f * width, (float)0.0f * width, (float)0.5 * width}, {1.0f,1.0f,1.0f,1.0f})
		};

		mesh->Indicies = {
			// front face
			0, 2, 3,
			0, 3, 1
		};

		mesh->Name = "Plane";

		return mesh;

	};
}