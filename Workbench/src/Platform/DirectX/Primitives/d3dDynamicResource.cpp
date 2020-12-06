#include "wbpch.h"
#include "d3dDynamicResource.h"
#include "Renderer/Primitives/Mesh.h"

namespace Workbench {
	d3dDynamicResource::d3dDynamicResource(
		ID3D12Device* device,
		D3D12MA::Allocator* allocator
	) :
		m_Device(device),
		m_Allocator(allocator)
	{};

	void d3dDynamicResource::UpdateResource(const void* data, uint64_t size, uint64_t elementSize, uint64_t elementCount) {

		if (m_Size != size || m_ElementSize != elementSize || m_ElementCount != elementCount) {

			m_Size = size;
			m_ElementSize = elementSize;
			m_ElementCount = elementCount;

			auto trueElementSize = elementSize;

			D3D12MA::ALLOCATION_DESC uploaderDesc = {};
			uploaderDesc.HeapType = D3D12_HEAP_TYPE_UPLOAD;

			D3D12MA_CREATE_RESOURCE(
				m_Allocator,
				&uploaderDesc,
				&CD3DX12_RESOURCE_DESC::Buffer(trueElementSize * elementCount),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				m_BufferAllocation,
				m_Buffer
			);
			m_MappedData = nullptr;
			m_Buffer->Map(0, NULL, &m_MappedData);
		}

		auto nonPaddedElementSize = m_Size / m_ElementCount;

		for (int i = 0; i < m_ElementCount; i++) {
			memcpy((char*)m_MappedData + (size_t)i*m_ElementSize, (const char*)data + (size_t)i*nonPaddedElementSize, nonPaddedElementSize);
		}
	}

	void d3dDynamicResource::ReleaseResource() {
		m_BufferAllocation.reset();
		m_Buffer.Reset();
	}
}
