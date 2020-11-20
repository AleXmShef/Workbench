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
			D3D12MA::Allocator* allocator,
			bool isConstantBuffer
		);

		void UpdateResource(const void* data, uint64_t size, uint64_t elementSize, uint64_t elementCount) override;
		void ReleaseResource() override;

	protected:
		template<typename T>
		using pCom = Microsoft::WRL::ComPtr<T>;
		template<typename T>
		using paCom = shared_ptr_with_custom_deleter < T, D3D12MA::Deleter>;

		ID3D12Device* m_Device = nullptr;
		D3D12MA::Allocator* m_Allocator = nullptr;

		pCom<ID3D12Resource> m_Buffer = nullptr;
		paCom<D3D12MA::Allocation> m_BufferAllocation;

		BYTE* m_MappedData = nullptr;

		bool m_IsConstantBuffer = false;
	};
}

