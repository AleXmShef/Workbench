#include "wbpch.h"
#include "d3dConstantBuffer.h"
#include "Logger/Logger.h"

namespace Workbench {
	d3dConstantBuffer::d3dConstantBuffer(
		ID3D12Device* device,
		D3D12MA::Allocator* allocator
	) : d3dDynamicResource(device, allocator)
	{
		
	};

	void d3dConstantBuffer::UpdateResource(const void* data, uint64_t size, uint64_t elementSize, uint64_t elementCount) {
		auto trueElementSize = CalcConstantBufferByteSize(elementSize);
		bool isDirty = (m_Size != size || m_ElementSize != trueElementSize || m_ElementCount != elementCount);

		d3dDynamicResource::UpdateResource(data, size, trueElementSize, elementCount);

		if (isDirty || m_Heap == nullptr) {
			WB_RENDERER_INFO("HEAP IS DIRTY");
			buildHeap();
			buildDescriptors();
		}
	}

	void d3dConstantBuffer::ReleaseResource() {
		d3dDynamicResource::ReleaseResource();
		WB_RENDERER_INFO("Releasing heap");
		m_Heap.Reset();
	}

	void d3dConstantBuffer::buildHeap() {
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.NumDescriptors = (UINT)m_ElementCount;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NodeMask = 0;

		auto result = m_Device->CreateDescriptorHeap(&heapDesc,
			IID_PPV_ARGS(&m_Heap));
		//handle error
	}

	void d3dConstantBuffer::buildDescriptors() {
		//auto trueElementSize = CalcConstantBufferByteSize(m_ElementSize);
		for (uint32_t i = 0; i < m_ElementCount; ++i) {
			//WB_RENDERER_LOG("Creating constant buffer view {}", i);
			D3D12_GPU_VIRTUAL_ADDRESS cbAddress = m_Buffer->GetGPUVirtualAddress();

			cbAddress += m_ElementSize * i;
			auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_Heap->GetCPUDescriptorHandleForHeapStart());
			handle.Offset(i, m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));


			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
			cbvDesc.BufferLocation = cbAddress;
			cbvDesc.SizeInBytes = (UINT)m_ElementSize;

			m_Device->CreateConstantBufferView(
				&cbvDesc,
				handle
			);
		}
	}

	ID3D12DescriptorHeap* d3dConstantBuffer::GetHeap() {
		return m_Heap.Get();
	}
}
