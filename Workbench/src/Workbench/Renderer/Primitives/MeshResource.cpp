#include "wbpch.h"
#include "MeshResource.h"

namespace Workbench {
	MeshResource::MeshResource(ConstantResource* vert, ConstantResource* ind, DynamicResource* consts) {
		m_VertexBuffer.reset(vert);
		m_IndexBuffer.reset(ind);
		m_ObjectConstants.reset(consts);
	}

	void MeshResource::UploadResource() {
		m_VertexBuffer->UploadResource();
		m_IndexBuffer->UploadResource();
		isDirty = false;
	}

	void MeshResource::ReleaseResource() {
		m_VertexBuffer->ReleaseResource();
		m_IndexBuffer->ReleaseResource();
	}

	void MeshResource::AddMesh(const UUID* entityId, const Mesh* mesh) {
		m_SubMeshes[entityId] = {};
		m_SubMeshes[entityId].BaseVertexLocation = (uint32_t)m_VertexArray.size();
		m_SubMeshes[entityId].VertexCount = (uint32_t)mesh->Verticies.size();
		m_SubMeshes[entityId].StartIndexLocation = (uint32_t)m_IndexArray.size();
		m_SubMeshes[entityId].IndexCount = (uint32_t)mesh->Indicies.size();

		m_VertexArray.insert(
			m_VertexArray.end(),
			mesh->Verticies.begin(),
			mesh->Verticies.end()
		);

		m_IndexArray.insert(
			m_IndexArray.end(),
			mesh->Indicies.begin(),
			mesh->Indicies.end()
		);

		m_VertexBuffer->UpdateResource(
			m_VertexArray.data(), sizeof(VertexColoredBasic) * m_VertexArray.size()
		);

		m_IndexBuffer->UpdateResource(
			m_IndexArray.data(), sizeof(uint32_t) * m_IndexArray.size()
		);
		isDirty = true;
	}

	void MeshResource::RemoveMesh(const UUID* entityId) {
		auto& subMesh = m_SubMeshes[entityId];

		m_VertexArray.erase(
			m_VertexArray.begin() + subMesh.BaseVertexLocation,
			m_VertexArray.begin() + subMesh.BaseVertexLocation + subMesh.VertexCount
		);

		m_IndexArray.erase(
			m_IndexArray.begin() + subMesh.StartIndexLocation,
			m_IndexArray.begin() + subMesh.StartIndexLocation + subMesh.IndexCount
		);

		m_VertexBuffer->UpdateResource(
			m_VertexArray.data(), sizeof(VertexColoredBasic) * m_VertexArray.size()
		);

		m_IndexBuffer->UpdateResource(
			m_IndexArray.data(), sizeof(uint32_t) * m_IndexArray.size()
		);

		isDirty = true;
	}

	Materials MeshResource::GetMaterial() const {
		return m_Material;
	}

	std::shared_ptr<ConstantResource> MeshResource::GetVertexBuffer()
	{
		return m_VertexBuffer;
	}

	std::shared_ptr<ConstantResource> MeshResource::GetIndexBuffer()
	{
		return m_IndexBuffer;
	}

	std::shared_ptr<DynamicResource> MeshResource::GetConstantBuffer()
	{
		return m_ObjectConstants;
	}

	const std::vector<VertexColoredBasic>* MeshResource::GetVertexArray()
	{
		return &m_VertexArray;
	}

	const std::vector<uint16_t>* MeshResource::GetIndexArray()
	{
		return &m_IndexArray;
	}

	const std::unordered_map<const Workbench::UUID*, MeshResource::SubMesh>* MeshResource::GetSubmeshes()
	{
		return &m_SubMeshes;
	}

	bool MeshResource::IsDirty() {
		return isDirty;
	}
}
