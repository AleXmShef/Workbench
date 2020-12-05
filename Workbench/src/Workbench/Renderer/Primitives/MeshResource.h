#pragma once
#include "wbpch.h"
#include "Utils/UUID.h"
#include "Renderer/Primitives/Mesh.h"
#include "Renderer/Primitives/Materials.h"
#include "Renderer/Primitives/ConstantResource.h"
#include "Renderer/Primitives/DynamicResource.h"

namespace Workbench {
	class MeshResource : public ConstantResource {
	public:
		struct SubMesh {
			uint32_t IndexCount = 0;
			uint32_t StartIndexLocation = 0;

			uint32_t VertexCount = 0;
			uint32_t BaseVertexLocation = 0;

			uint32_t InstanceCount = 0;
		};

		MeshResource(ConstantResource* vert, ConstantResource* ind, DynamicResource* consts);

		void UpdateResource() = delete;
		virtual void UploadResource() override;
		virtual void ReleaseResource() override;
		void AddMesh(const UUID* entityId, const Mesh* mesh);
		void RemoveMesh(const UUID* entityId);

		Materials GetMaterial() const;

		std::shared_ptr<ConstantResource> GetVertexBuffer();
		std::shared_ptr<ConstantResource> GetIndexBuffer();
		std::shared_ptr<DynamicResource> GetConstantBuffer();

		const std::vector<VertexColoredBasic>* GetVertexArray();
		const std::vector<uint16_t>* GetIndexArray();

		const std::unordered_map<const Workbench::UUID*, SubMesh>* GetSubmeshes();

		bool IsDirty();

	protected:

		std::unordered_map<const UUID*, SubMesh> m_SubMeshes;

		std::shared_ptr<ConstantResource> m_VertexBuffer = nullptr;
		std::vector<VertexColoredBasic> m_VertexArray;

		std::shared_ptr<ConstantResource> m_IndexBuffer = nullptr;
		std::vector<uint16_t> m_IndexArray;

		std::shared_ptr<DynamicResource> m_ObjectConstants = nullptr;

		Materials m_Material = Materials::GenericFlat;

		bool isDirty = false;
	};
}

