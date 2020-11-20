#include "wbpch.h"
#include "d3dConstantResource.h"

namespace Workbench {
	d3dConstantResource::d3dConstantResource(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* cmdList,
		D3D12MA::Allocator* allocator
	) : 
		m_Device(device),
		m_CmdList(cmdList),
		m_Allocator(allocator)
	{};

	void d3dConstantResource::UpdateResource(const void* data, uint64_t size) {

		D3DCreateBlob(size, m_Data.GetAddressOf());
		CopyMemory(m_Data->GetBufferPointer(), data, size);

		m_SubResourceData = {};
		m_SubResourceData.pData = m_Data->GetBufferPointer();
		m_SubResourceData.RowPitch = m_Data->GetBufferSize();
		m_SubResourceData.SlicePitch = m_SubResourceData.RowPitch;
	}

	void d3dConstantResource::UploadResource() {
		{
			D3D12MA::ALLOCATION_DESC uploaderDesc = {};
			uploaderDesc.HeapType = D3D12_HEAP_TYPE_UPLOAD;

			D3D12MA_CREATE_RESOURCE(
				m_Allocator,
				&uploaderDesc,
				&CD3DX12_RESOURCE_DESC::Buffer(m_SubResourceData.RowPitch),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				m_UploaderAllocation,
				m_Uploader
			);
		}

		{
			D3D12MA::ALLOCATION_DESC bufferDesc = {};

			bufferDesc.HeapType = D3D12_HEAP_TYPE_DEFAULT;

			D3D12MA_CREATE_RESOURCE(
				m_Allocator,
				&bufferDesc,
				&CD3DX12_RESOURCE_DESC::Buffer(m_SubResourceData.RowPitch),
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				m_BufferAllocation,
				m_Buffer
			);
		}

		m_CmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_Buffer.Get(),
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
		UpdateSubresources<1>(m_CmdList, m_Buffer.Get(), m_Uploader.Get(), 0, 0, 1, &m_SubResourceData);
		m_CmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_Buffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));
	}

	void d3dConstantResource::ReleaseResource() {

		m_Data = nullptr;
		
		if (m_UploaderAllocation != nullptr) {
			m_UploaderAllocation.reset();
		}
		if (m_Uploader != nullptr)
			m_Uploader = nullptr;
		
		if (m_BufferAllocation != nullptr) {
			m_BufferAllocation.reset();
		}
		if(m_Buffer != nullptr)
			m_Buffer = nullptr;
	}
}
