#include "wbpch.h"
#include "MeshResource.h"

namespace Workbench {
	MeshResource::MeshResource(ConstantResource* vert, ConstantResource* ind) {
		m_VertexBuffer.reset(vert);
		m_IndexBuffer.reset(ind);
	}

	void MeshResource::UploadResource() {
		m_VertexBuffer->UploadResource();
		m_IndexBuffer->UploadResource();
	}

	void MeshResource::ReleaseResource() {
		m_VertexBuffer->ReleaseResource();
		m_IndexBuffer->ReleaseResource();
	}

	void MeshResource::AddMesh(const Mesh* mesh) {
		m_SubMeshes[mesh->Name] = {};
		m_SubMeshes[mesh->Name].BaseVertexLocation = (uint32_t)m_VertexArray.size();
		m_SubMeshes[mesh->Name].VertexCount = (uint32_t)mesh->Verticies.size();
		m_SubMeshes[mesh->Name].StartIndexLocation = (uint32_t)m_IndexArray.size();
		m_SubMeshes[mesh->Name].IndexCount = (uint32_t)mesh->Indicies.size();

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
	}

	void MeshResource::RemoveMesh(std::string& meshName) {
		auto& subMesh = m_SubMeshes[meshName];
		
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
	}
}
