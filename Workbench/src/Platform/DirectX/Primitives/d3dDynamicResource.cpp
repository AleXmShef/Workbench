#include "wbpch.h"
#include "d3dDynamicResource.h"

namespace Workbench {
	d3dDynamicResource::d3dDynamicResource(
		ID3D12Device* device,
		D3D12MA::Allocator* allocator,
		bool isConstantBuffer
	) :
		m_Device(device),
		m_Allocator(allocator),
		m_IsConstantBuffer(isConstantBuffer)
	{};

	void d3dDynamicResource::UpdateResource(const void* data, uint64_t size, uint64_t elementSize, uint64_t elementCount) {

		auto trueElementSize = elementSize;

		if (m_IsConstantBuffer)
			trueElementSize = CalcConstantBufferByteSize(elementSize);

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

		m_Buffer->Map(0, nullptr, reinterpret_cast<void**>(&m_MappedData));

		memcpy(&m_MappedData[0], data, size);
	}

	void d3dDynamicResource::ReleaseResource() {
		m_BufferAllocation.reset();
		m_Buffer.Reset();
	}
}
