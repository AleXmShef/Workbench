#pragma once
#include "wbpch.h"
#include "Utils/Utils.h"
#include "Platform/DirectX/Utils.h"
#include "Platform/DirectX/Primitives/d3dDynamicResource.h"

namespace Workbench {
	class d3dConstantBuffer : public d3dDynamicResource {
	public:
		d3dConstantBuffer(
			ID3D12Device* device,
			D3D12MA::Allocator* allocator
		);

		virtual void UpdateResource(const void* data, uint64_t size, uint64_t elementSize, uint64_t elementCount) override;
		ID3D12DescriptorHeap* GetHeap();

	protected:
		void buildHeap();
		void buildDescriptors();

	protected:
		pCom<ID3D12DescriptorHeap> m_Heap;
	};
}

