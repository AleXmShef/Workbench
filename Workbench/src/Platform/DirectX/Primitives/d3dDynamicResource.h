#pragma once
#include "wbpch.h"
#include "Utils/Utils.h"
#include "Platform/DirectX/Utils.h"

#include "Renderer/Primitives/DynamicResource.h"

namespace Workbench {
	class d3dDynamicResource : public DynamicResource {
	public:
		d3dDynamicResource(
			ID3D12Device* device,
			D3D12MA::Allocator* allocator
		);

		virtual void UpdateResource(const void* data, uint64_t size, uint64_t elementSize, uint64_t elementCount) override;
		virtual void ReleaseResource() override;

	protected:

		ID3D12Device* m_Device = nullptr;
		D3D12MA::Allocator* m_Allocator = nullptr;

		pCom<ID3D12Resource> m_Buffer = nullptr;
		paCom<D3D12MA::Allocation> m_BufferAllocation;

		void* m_MappedData = nullptr;

		uint64_t m_Size = 0;
		uint64_t m_ElementSize = 0;
		uint64_t m_ElementCount = 0;
	};
}

