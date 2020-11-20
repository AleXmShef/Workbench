#pragma once
#include "wbpch.h"
#include "Renderer/Primitives/Mesh.h"
#include "Renderer/Primitives/ConstantResource.h"

namespace Workbench {
	class MeshResource : public ConstantResource {
	public:
		MeshResource(ConstantResource* vert, ConstantResource* ind);

		virtual void UploadResource() override;
		virtual void ReleaseResource() override;
		virtual void AddMesh(const Mesh* mesh);
		virtual void RemoveMesh(std::string& meshName);

	protected:
		struct SubMesh {
			uint32_t IndexCount = 0;
			uint32_t StartIndexLocation = 0;

			uint32_t VertexCount = 0;
			uint32_t BaseVertexLocation = 0;
		};

		std::unordered_map<std::string, SubMesh> m_SubMeshes;

		std::unique_ptr<ConstantResource> m_VertexBuffer = nullptr;
		std::vector<VertexColoredBasic> m_VertexArray;

		std::unique_ptr<ConstantResource> m_IndexBuffer = nullptr;
		std::vector<uint32_t> m_IndexArray;
	};
}

